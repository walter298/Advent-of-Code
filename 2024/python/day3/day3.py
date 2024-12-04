import bisect
import re

def parse_mul_match(match : str) -> int:
    a, b = map(int, match.strip().split(','))
    return a * b

MUL_PATTERN = r'mul\(([0-9,]+)\)'

def sum_mul_nums_in_line(str : str) -> int:
    total = 0
    mul_matches = re.findall(pattern=MUL_PATTERN, string=str)
    for match in mul_matches:
        total += parse_mul_match(match)
    return total

MUL  = 0
DO   = 1
DONT = 2

def get_parse_state(match):
    if match.group(0)[0] == 'm':
        return MUL
    elif match.group(0) == "do()":
        return DO
    else:
        return DONT

def sum_mul_nums_in_line_with_enable_commands(concat_lines : str) -> int:
    total = 0

    matches  = list(re.finditer(pattern=r'mul\(([0-9,]+)\)|do\(\)|don\'t\(\)', string=concat_lines))

    curr_command = DO

    for match in matches:
        parsed_elem = get_parse_state(match)
        if parsed_elem == MUL and curr_command == DO:
            total += parse_mul_match(match.group(1))
        else:
            curr_command = parsed_elem
    return total

def __main__():
    total = 0
    with open('day3_input.txt', mode='r') as file:
        lines = ''.join(file.readlines())
        print(sum_mul_nums_in_line(lines))
        print(sum_mul_nums_in_line_with_enable_commands(lines))

if __name__ == '__main__':
    __main__()