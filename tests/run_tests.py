import subprocess
from itertools import zip_longest

RED = '\033[31m'
GREEN = '\033[32m'
YELLOW = '\033[33m'
RESET = '\033[0m'

tests = [
    ('test_var.nv', ['nil', '1', '11', '90 3.14', '3.14 90', 'false', 'true', 'Hello, World!', 'global', 'scope1', 'scope2', 'scope1', 'global']),
    ('test_control_flow.nv', ['1: True', '3: True', '4: False', '5: True', '6: Maybe', '7: False', '8: False', '9: False', '10: False', '11: True', '12: False', '13: True', '14: True', '15: True', '16: False', '17: True']),
    ('test_loop.nv', ['0', '1', '2', '3', '4', '-5', '-3', '-1', '1', '2', '3', 'Hello, im string', '20', '40', '60', '80', '0 0', '0 1', '1 0', '1 1', '0 1', '0 2', '0 3', '1 2', '1 3', '2 3']),
    ('test_func.nv', ['2', 'nil', '10', '8', '120', '6765', 'im global', 'im local'])
]

def run_test(script_path: str, expected_output: list[str]) -> int:
    result = subprocess.run(['nova', f"tests/{script_path}"],
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            text=True)
    
    if result.returncode == -11:
        print(f"{RED}[✗]{RESET} {script_path} failed due to segmentation fault")
        return 0
    
    output = result.stdout.strip().split('\n')
    
    passed = True
    lines = []

    for i, (expected_line, output_line) in enumerate(zip_longest(expected_output, output, fillvalue='?')):
        if expected_line != output_line or output_line == '?':
            lines.append(i)
            passed = False
    
    if passed:
        print(f"{GREEN}[✓]{RESET} {script_path} passed")
        return 1
    else:
        print(f"{RED}[✗]{RESET} {script_path} failed")
        print(f"Expect:")
        for i, expected_line in enumerate(expected_output):
            if i in lines:
                print(f"{YELLOW}{expected_line}{RESET}")
            else:
                print(f"{expected_line}")
        print(f"Got:")
        for i, output_line in enumerate(output):
            if i in lines:
                print(f"{RED}{output_line}{RESET}")
            else:
                print(f"{output_line}")
        print(f"Stderr: '{result.stderr.strip()}'")
        return 0

def main():
    test_count = len(tests)
    pass_count = 0

    for path, expected in tests:
        pass_count += run_test(path, expected)    
    
    if pass_count == test_count:
        print(f"{GREEN}All tests passed{RESET}")
    else:
        print(f"{RED}{pass_count} out of {test_count} test passed{RESET}")
    
if __name__ == '__main__':
    main()