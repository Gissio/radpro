# MCU renderer
# textproc
# (C) 2025 Gissio

import argparse
import re


def parse_codepoint_set(codepoint_set):
    codepoints = set()

    if codepoint_set:
        codepoint_ranges = codepoint_set.split(',')

        for codepoint_range in codepoint_ranges:
            codepoint_range_items = codepoint_range.split('-')
            if len(codepoint_range_items) == 1:
                value = int(codepoint_range_items[0], 0)

                if 0 <= value:
                    codepoints.add(value)
                else:
                    raise RuntimeError('invalid codepoint set')
            elif len(codepoint_range_items) == 2:
                start = int(codepoint_range_items[0], 0)
                end = int(codepoint_range_items[1], 0)

                if 0 <= start <= end:
                    codepoints.update(range(start, end + 1))
                else:
                    raise RuntimeError('invalid codepoint set')
            else:
                raise RuntimeError('invalid codepoint set')

    return codepoints


def build_codepoint_set(codepoints):
    if not codepoints:
        return ''

    def build_codepoint_range(start, end):
        if start == end:
            return f'{hex(start)}'
        else:
            return f'{hex(start)}-{hex(end)}'

    codepoint_list = list(codepoints)
    codepoint_list.sort()

    codepoint_ranges = []
    start = codepoint_list[0]
    prev = codepoint_list[0]

    for codepoint in codepoint_list[1:]:
        if codepoint != prev + 1:
            codepoint_ranges.append(build_codepoint_range(start, prev))
            start = codepoint
        prev = codepoint

    codepoint_ranges.append(build_codepoint_range(start, prev))

    return codepoint_ranges


def escape_chars(s):
    escapes = {
        r'\n': '\n',
        r'\t': '\t',
        r'\r': '\r',
        r'\b': '\b',
        r'\f': '\f',
        r'\v': '\v',
        r'\\': '\\',
        r'\"': '\"',
        r'\'': '\'',
        r'\0': '\0',
    }

    def replace_hex(match):
        hex_value = match.group(1)
        return chr(int(hex_value, 16))

    def replace_octal(match):
        octal_value = match.group(1)
        return chr(int(octal_value, 8))

    # Replace hex escapes
    s = re.sub(r'\\x([0-9a-fA-F]{1,2})', replace_hex, s)

    # Replace octal escapes
    s = re.sub(r'\\([0-7]{1,3})', replace_octal, s)

    # Replace standard escape sequences
    for esc, char in escapes.items():
        s = s.replace(esc, char)

    return s


def filter_c_strings(text):
    # Remove single-line comments (// ...)
    text = re.sub(r'//.*?\n', '', text)

    # Remove multi-line comments (/*...*/)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)

    # Find all strings in double quotes, including multi-line strings
    strings = re.findall(r'"((?:[^"\\]|\\.)*?)"', text, re.DOTALL)

    # Escape strings
    processed_strings = [escape_chars(s) for s in strings]

    return ''.join(processed_strings)


def main():
    # Parse arguments
    parser = argparse.ArgumentParser(
        description='Calculates a fontconv codepoint set from one or several text files.')
    parser.add_argument('-c', '--c-strings',
                        dest='c_strings',
                        action='store_true',
                        help=f'parses input files as C-language code, searching only quoted text')
    parser.add_argument('-s', '--codepoint-set',
                        dest='codepoint_set',
                        default='',
                        help='adds Unicode codepoints (e.g. "32-255" or "0x2e,0x30-0x39")')
    parser.add_argument('filename',
                        nargs='+')
    args = parser.parse_args()

    try:
        # Codepoint set
        codepoints = parse_codepoint_set(args.codepoint_set)

        # Files
        for filename in args.filename:
            text = open(filename, 'rt', encoding='utf-8').read()

            if args.c_strings:
                text = filter_c_strings(text)

            codepoints.update({ord(char) for char in text})

        # Build ranges
        codepoint_set = build_codepoint_set(codepoints)

        # Print result
        print(','.join(codepoint_set))

    except Exception as e:
        print('error: ' + str(e))
        exit(1)


if __name__ == '__main__':
    main()
