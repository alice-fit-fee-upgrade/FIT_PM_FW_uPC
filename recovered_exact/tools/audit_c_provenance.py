#!/usr/bin/env python3
"""Check manual ASM-byte accounting against GCC's APP/NOAPP provenance.

Recompile accepted C sources to assembly using their exact Makefile flags.
No emulator or golden-byte substitution is involved. Subsections are ordered
as GNU as does, including appended shared error tails.
"""
from pathlib import Path
from collections import defaultdict
import json
import re
import shlex
import subprocess

PROJECT = Path(__file__).resolve().parents[1]
BUILD = PROJECT / 'build'


def compile_assembly():
    destination = BUILD / 'c_provenance'
    destination.mkdir(exist_ok=True)
    commands = subprocess.check_output(
        ['make', '-B', '-n', 'all'], cwd=PROJECT, text=True).splitlines()
    sources = {}
    for command in commands:
        if not command.startswith('avr-gcc ') or ' -c src/' not in command:
            continue
        args = shlex.split(command)
        source = next(arg for arg in args if arg.startswith('src/') and arg.endswith('.c'))
        output = destination / (Path(source).stem + '.s')
        args[args.index('-c')] = '-S'
        args[args.index('-o') + 1] = str(output)
        subprocess.run(args, cwd=PROJECT, check=True, capture_output=True)
        sources[source] = output
    return sources


def instruction_provenance(path):
    groups = defaultdict(list)
    section, subsection, stack, inline = '.text', 0, [], False
    for line in path.read_text().splitlines():
        text = line.strip()
        if text in ('#APP', '/* #APP */'):
            inline = True
            continue
        if text in ('#NOAPP', '/* #NOAPP */'):
            inline = False
            continue
        if not text or text.startswith(('#', ';', '/*')):
            continue
        directive = text.split()[0]
        if directive in ('.pushsection', '.section'):
            if directive == '.pushsection':
                stack.append((section, subsection))
            section, subsection = text.split()[1].split(',')[0], 0
            continue
        if text == '.text':
            section, subsection = '.text', 0
            continue
        if directive == '.subsection':
            subsection = int(text.split()[1])
            continue
        if text == '.popsection':
            section, subsection = stack.pop()
            continue
        text = re.sub(r'^([\w.$]+):\s*', '', text)
        if not text or text.startswith('.') or '=' in text:
            continue
        mnemonic = text.split()[0]
        assert re.fullmatch('[a-z]+', mnemonic), (path, text)
        # ATxmega128A3U: these are the four 32-bit AVR instruction forms.
        width = 4 if mnemonic in ('call', 'jmp', 'lds', 'sts') else 2
        groups[(section, subsection)].append((width, inline, mnemonic))
    result = defaultdict(list)
    for (section, _), rows in sorted(groups.items()):
        result[section] += rows
    return result


def main():
    manifest = json.loads((PROJECT / 'manifest.json').read_text())
    sources = compile_assembly()
    parsed = {source: instruction_provenance(path) for source, path in sources.items()}
    report, helper_bytes, mismatches = [], 0, []
    primitive_bytes = 0
    avr_primitives = {"cli", "sei", "nop", "swap", "bst", "bld"}
    for entry in manifest['accepted']:
        rows = parsed[entry['source']][entry['section']]
        address = entry['address']
        entry_primitives = 0
        expected, actual, differences = set(), set(), []
        for region in entry.get('asm_helper_ranges', []):
            expected.update(range(region['start'], region['end_exclusive']))
        for width, inline, mnemonic in rows:
            instruction_bytes = set(range(address, address + width))
            if inline:
                actual.update(instruction_bytes)
            elif mnemonic in avr_primitives:
                entry_primitives += width
            if (instruction_bytes & expected) != (instruction_bytes if inline else set()):
                differences.append({'address': address, 'instruction': mnemonic,
                                    'gcc_inline_asm': inline})
            address += width
        assert address == entry['end_exclusive'], (entry['symbol'], address)
        if expected != actual:
            mismatches.append({'symbol': entry['symbol'], 'differences': differences})
        helper_bytes += len(actual)
        primitive_bytes += entry_primitives
        report.append({'symbol': entry['symbol'], 'inline_asm_bytes': len(actual),
                       'compiler_generated_avr_primitive_bytes': entry_primitives,
                       'compiler_bytes': address - entry['address'] - len(actual)})
    output = {'method': 'GCC APP/NOAPP instruction provenance with GNU subsection ordering',
              'accepted_entries': len(report), 'translation_units': len(sources),
              'inline_asm_bytes': helper_bytes,
              'compiler_generated_avr_primitive_bytes': primitive_bytes,
              'avr_primitive_mnemonics': sorted(avr_primitives),
              'mismatches': mismatches,
              'application': report}
    (BUILD / 'c_provenance.json').write_text(json.dumps(output, indent=2) + '\n')
    assert not mismatches, mismatches
    inventory = json.loads((BUILD / 'function_inventory.json').read_text())
    assert inventory['compiler_generated_avr_primitive_bytes'] == primitive_bytes, (
        'instruction-index and GCC primitive-byte accounting disagree', primitive_bytes)
    print(f'C/ASM provenance: {len(report)} entries, {helper_bytes} inline ASM bytes; '
          'manifest matches GCC assembly')


if __name__ == '__main__':
    main()
