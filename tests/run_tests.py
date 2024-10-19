import subprocess
import os
from itertools import zip_longest

RED = '\033[31m'
GREEN = '\033[32m'
YELLOW = '\033[33m'
RESET = '\033[0m'

SCRIPTS_DIR = 'tests/scripts'
GOLDEN_DIR = 'tests/golden'
RESULTS_DIR = 'tests/results'
INTERPRETER = 'nova'

def run_test(name: str) -> None:
    script_path = os.path.join(SCRIPTS_DIR, name)
    stdout_path = os.path.join(RESULTS_DIR, f'{name[:-3]}.out')
    stderr_path = os.path.join(RESULTS_DIR, f'{name[:-3]}.err')

    with open(stdout_path, 'w') as stdout_file, open(stderr_path, 'w') as stderr_file:
        subprocess.run([INTERPRETER, script_path], stdout=stdout_file, stderr=stderr_file)

def compare(name: str) -> bool:
    golden_stdout_path = os.path.join(GOLDEN_DIR, f'{name[:-3]}.out')
    golden_stderr_path = os.path.join(GOLDEN_DIR, f'{name[:-3]}.err')
    result_stdout_path = os.path.join(RESULTS_DIR, f'{name[:-3]}.out')
    result_stderr_path = os.path.join(RESULTS_DIR, f'{name[:-3]}.err')

    with open(golden_stdout_path) as f1, open(golden_stderr_path) as f2:
        golden_stdout = f1.read()
        golden_stderr = f2.read()

    with open(result_stdout_path) as f1, open(result_stderr_path) as f2:
        result_stdout = f1.read()
        result_stderr = f2.read()

    return golden_stdout == result_stdout and golden_stderr == result_stderr

def print_diff(name: str) -> None:
    golden_stdout_path = os.path.join(GOLDEN_DIR, f'{name[:-3]}.out')
    golden_stderr_path = os.path.join(GOLDEN_DIR, f'{name[:-3]}.err')
    result_stdout_path = os.path.join(RESULTS_DIR, f'{name[:-3]}.out')
    result_stderr_path = os.path.join(RESULTS_DIR, f'{name[:-3]}.err')

    with open(golden_stdout_path, 'r') as f1, open(result_stdout_path, 'r') as f2:
        for i, (golden_line, result_line) in enumerate(zip_longest(f1, f2, fillvalue='(no out)\n')):
            if golden_line != result_line:
                print(f'Line {i}')
                print(f'{YELLOW}Expected:{RESET} {golden_line}', end='')
                print(f'{RED}Got:{RESET}      {result_line}', end='')

    with open(golden_stderr_path, 'r') as f1, open(result_stderr_path, 'r') as f2:
        for i, (golden_line, result_line) in enumerate(zip_longest(f1, f2, fillvalue='(no err)\n')):
            if golden_line != result_line:
                print(f'Line {i}')
                print(f'{YELLOW}Expected:{RESET} {golden_line}', end='')
                print(f'{RED}Got:{RESET}      {result_line}', end='')

def main():
    scripts = [f for f in os.listdir(SCRIPTS_DIR)]
    test_count = 0
    pass_count = 0

    for script in scripts:
        test_count += 1
        run_test(script)

        if compare(script):
            pass_count += 1
            print(f'{GREEN}[✓]{RESET} {script} passed')
        else:
            print(f'{RED}[✗]{RESET} {script} failed')
            print_diff(script)
        
    if pass_count == test_count:
        print(f'{GREEN}All tests passed{RESET}')
    else:
        print(f'{RED}{pass_count} out of {test_count} tests passed{RESET}')
    
if __name__ == '__main__':
    main()