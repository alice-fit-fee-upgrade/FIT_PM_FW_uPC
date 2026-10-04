#!/usr/bin/env python3
"""Exhaustive comparison to actual original AVR opcode execution for all inputs."""
import ctypes,pathlib,subprocess,tempfile
root=pathlib.Path(__file__).resolve().parents[1]
g=(root/'reference/flash_golden.bin').read_bytes()
assert g[0x2720:0x272a]==bytes.fromhex('0f70005d0a3308f0095f')
def original(r16):
    # ANDI 0x0F; SUBI 0xD0; CPI 0x3A; BRCS +2; SUBI 0xF9.
    r16 &= 15
    r16=(r16-0xd0)&255
    if r16>=0x3a: r16=(r16-0xf9)&255
    return r16
with tempfile.TemporaryDirectory() as tmp:
    lib=pathlib.Path(tmp)/'hex.so'
    subprocess.run(['cc','-shared','-fPIC',str(root/'source_recovery/hex_digit.c'),'-o',str(lib)],check=True)
    f=ctypes.CDLL(str(lib)).pm_hex_digit; f.argtypes=[ctypes.c_uint8]; f.restype=ctypes.c_uint8
    for v in range(256): assert f(v)==original(v),(v,f(v),original(v))
print('PASS: 256 inputs; C matches original instruction arithmetic. Flags/send/ABI not validated.')
