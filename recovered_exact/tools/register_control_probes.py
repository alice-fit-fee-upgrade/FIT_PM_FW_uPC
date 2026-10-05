#!/usr/bin/env python3
"""Record actual AVR GCC pragma/register support; never reads firmware bytes."""
import argparse
import json
from pathlib import Path
import subprocess
import tempfile

FLAGS = ['-mmcu=atxmega128a3u', '-Os', '-Wall', '-Wextra', '-Werror']
REGISTER_FLAGS = [f'-fcall-saved-r{i}' for i in (16, 17, 18, 20, 21, 30, 31)]
CASES = {
    'target_fixed': ('#pragma GCC target("fixed-r18")\nvoid test(void) {}', []),
    'optimize_fixed': ('#pragma GCC optimize("fixed-r18")\nvoid test(void) {}', []),
    'attribute_fixed': ('__attribute__((optimize("fixed-r18"))) void test(void) {}', []),
    'target_short_calls': ('#pragma GCC target("short-calls")\nvoid test(void) {}', []),
    'pragma_O1': ('#pragma GCC push_options\n#pragma GCC optimize("O1")\nvoid test(void) {}\n#pragma GCC pop_options', []),
    'global_word_copy': ('register uint16_t source asm("r28"); register uint16_t dest asm("r20"); void test(void) { dest = source; }', REGISTER_FLAGS),
    'global_word_add': ('register uint16_t cursor asm("r28"); void test(void) { cursor += 2; }', REGISTER_FLAGS),
    'global_word_subtract': ('register uint16_t cursor asm("r28"); void test(void) { cursor -= 8; }', REGISTER_FLAGS),
    'local_opaque_mask': ('void test(void) { register uint8_t data asm("r17"), mask asm("r18") = 0x80; asm volatile("" : "=r"(data), "+r"(mask)); data ^= mask; asm volatile("" : "+r"(data)); }', ['-fcall-used-r17', '-fcall-used-r18']),
    'global_pointer_postincrement': ('register uint8_t *cursor asm("r30"); register uint8_t data asm("r16"); void test(void) { data = *cursor++; }', REGISTER_FLAGS),
}


def run(compiler):
    report = {'compiler': subprocess.check_output([compiler, '--version'], text=True).splitlines()[0], 'flags': FLAGS, 'scope': 'isolated compiler support/encoding probes; not full-image conversion acceptance', 'cases': []}
    with tempfile.TemporaryDirectory(prefix='pm-register-probe-') as temp:
        directory = Path(temp)
        for name, (body, extra) in CASES.items():
            source = '#include <stdint.h>\n' + body + '\n'
            inp, out = directory / (name + '.c'), directory / (name + '.s')
            inp.write_text(source)
            result = subprocess.run([compiler, *FLAGS, *extra, '-S', str(inp), '-o', str(out)], text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            record = {'name': name, 'source': source, 'extra_flags': extra, 'exit_code': result.returncode, 'diagnostics': result.stdout.replace(str(directory), '<probe>')}
            if result.returncode == 0:
                assembly = out.read_text().split('test:', 1)[1].split('.size', 1)[0]
                record['assembly'] = assembly.strip()
            report['cases'].append(record)
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', default='avr-gcc')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.write_text(json.dumps(run(args.compiler), indent=2) + '\n')
