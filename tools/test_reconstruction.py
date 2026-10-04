#!/usr/bin/env python3
"""Behavior checks for forensic normalization and comparison failure modes."""
import pathlib,tempfile,unittest,subprocess
from memory_image import read_hex,canonical
from compare_flash import report

def record(address,kind,payload):
    b=bytes([len(payload)])+address.to_bytes(2,'big')+bytes([kind])+payload
    return ':'+(b+bytes([-sum(b)&255])).hex().upper()+'\n'

class ForensicTools(unittest.TestCase):
    def parse(self,text):
        with tempfile.TemporaryDirectory() as d:
            p=pathlib.Path(d)/'image.hex'; p.write_text(text); return read_hex(p)
    def test_segment_and_linear_addresses(self):
        for kind,payload in [(2,b'\x20\x00'),(4,b'\x00\x02')]:
            m,_=self.parse(record(0,kind,payload)+record(0x1a0,0,b'\x7f\xc0')+record(0,1,b''))
            self.assertEqual(m,{0x201a0:0x7f,0x201a1:0xc0})
    def test_sparse_equals_explicit_erased_cells(self):
        sparse,_=self.parse(record(1,0,b'\x42')+record(0,1,b''))
        full,_=self.parse(record(0,0,b'\xff\x42\xff')+record(0,1,b''))
        self.assertEqual(canonical(sparse,3),canonical(full,3))
    def test_corruption_and_conflicting_overlap_rejected(self):
        with self.assertRaises(ValueError): self.parse(':010000004200\n:00000001FF\n')
        with self.assertRaises(ValueError): self.parse(record(0,0,b'\x42')+record(0,0,b'\x43')+record(0,1,b''))
        with self.assertRaises(ValueError): self.parse(record(0,0,b'\x42'))
        with self.assertRaises(ValueError): canonical({3:42},3)
    def test_erased_tail_does_not_hide_used_mismatch(self):
        g=b'\x42'+b'\xff'*999; r=b'\x43'+b'\xff'*999
        text,different=report(g,r); self.assertTrue(different)
        self.assertIn('1 bytes, 1 differing, 0.000000% identical',text)
        self.assertIn('Number of mismatch regions: 1',text)
    def test_added_program_in_erased_space_is_counted(self):
        text,different=report(b'\x42\xff',b'\x42\x43'); self.assertTrue(different)
        self.assertIn('PROGRAMMED UNION (golden or rebuilt non-FF): 2 bytes, 1 differing',text)
    def test_instruction_branch_change_is_reported(self):
        root=pathlib.Path(__file__).resolve().parents[1]
        g=(root/'reference/flash_golden.bin').read_bytes(); r=bytearray(g); r[0x201fe] ^= 0x08 # Change BREQ displacement, retain branch opcode
        with tempfile.TemporaryDirectory() as d:
            gp=pathlib.Path(d)/'g.bin'; rp=pathlib.Path(d)/'r.bin'; gp.write_bytes(g); rp.write_bytes(r)
            out=subprocess.check_output(['python3',str(root/'tools/instruction_diff.py'),str(gp),str(rp)],text=True)
        self.assertIn('0x0201fe',out); self.assertIn('D relative branch',out); self.assertNotIn('windows: 0',out)

if __name__=='__main__': unittest.main()
