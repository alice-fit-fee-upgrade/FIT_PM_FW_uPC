#!/usr/bin/env python3
"""Account for retained inline instructions using the verified golden index.

This is a source/provenance inventory, not a new code/data classifier. The root
exact-check must first validate the canonical image and manifest helper ranges.
Addresses and lengths are FLASH bytes, never AVR program-counter word addresses.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path

PROJECT = Path(__file__).resolve().parents[1]
ROOT = PROJECT.parent


def inventory():
    manifest_path = PROJECT / 'manifest.json'
    manifest = json.loads(manifest_path.read_text())
    index = json.loads((ROOT / 'docs/instruction_index.json').read_text())
    owners = {}
    for entry in manifest['accepted']:
        for region in entry.get('asm_helper_ranges', []):
            for address in range(region['start'], region['end_exclusive']):
                assert address not in owners, f'overlapping helper at {address:#x}'
                owners[address] = (entry['symbol'], entry['source'])
    counts, widths, covered = Counter(), Counter(), set()
    examples = {}
    per_function = {}
    for instruction in index:
        address = instruction['address']
        size = len(bytes.fromhex(instruction['bytes']))
        addresses = set(range(address, address + size))
        if not addresses & owners.keys():
            continue
        assert addresses <= owners.keys(), f'partial helper instruction at {address:#x}'
        assert len({owners[a] for a in addresses}) == 1
        symbol, source = owners[address]
        mnemonic = instruction['instruction'].split()[0]
        counts[mnemonic] += 1
        widths[mnemonic] += size
        covered.update(addresses)
        examples.setdefault(mnemonic, [])
        if len(examples[mnemonic]) < 3:
            examples[mnemonic].append(dict(address=address, source=source,
                golden_bytes=instruction['bytes'], instruction=instruction['instruction']))
        function = per_function.setdefault(symbol, dict(source=source,
            inline_asm_bytes=0, instructions=0))
        function['inline_asm_bytes'] += size
        function['instructions'] += 1
    assert covered == owners.keys(), 'golden index does not cover all helper bytes'
    return dict(scope='inline ASM inside accepted C regions; pure ASM/boot excluded',
        address_units='FLASH bytes',
        manifest_sha256=hashlib.sha256(manifest_path.read_bytes()).hexdigest(),
        inline_asm_bytes=len(covered), instructions=sum(counts.values()),
        opcodes=[dict(mnemonic=op, bytes=width, instructions=counts[op],
                     examples=examples[op]) for op, width in widths.most_common()],
        functions=per_function)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, help='JSON report; stdout otherwise')
    args = parser.parse_args()
    result = json.dumps(inventory(), indent=2) + '\n'
    if args.output:
        args.output.write_text(result)
    else:
        print(result, end='')
