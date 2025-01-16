import subprocess
import os
import time

RED = '\033[31m'
GREEN = '\033[32m'
YELLOW = '\033[33m'
RESET = '\033[0m'

TEST_DIR = 'tests'
INTERPRETER = 'nova'

def run_test(name: str) -> int:
    script_path = os.path.join(TEST_DIR, name)

    start_time = time.time()

    result = subprocess.run([INTERPRETER, script_path], capture_output=True)

    missing = 0
    for line in result.stdout.splitlines():
        if b'missing' in line:
            missing = int(line.split()[-1])

    end_time = time.time()

    duration = end_time - start_time

    return result.returncode, duration, missing 

def main():
    print("\nStatus File Name                      Time    Details")
    tests = [f for f in os.listdir(TEST_DIR) if f.startswith('test_')]
    test_count = 0
    pass_count = 0
    total_duration = 0
    total_missing = 0

    for test in tests:
        test_count += 1
        returncode, duration, missing = run_test(test)
        total_duration += duration
        total_missing += missing 

        if returncode == 0: 
            pass_count += 1
            if missing == 0:
                print(f'{GREEN}  [✓] {RESET} {test:<30} {duration:.3f}s')
            else:
                print(f'{YELLOW}  [!] {RESET} {test:<30} {duration:.3f}s  missing {missing} {'cases' if missing > 1 else 'case'}')
        else:
            print(f'{RED}  [✗] {RESET} {test:<30} {duration:.3f}s')

    print()
    if pass_count == test_count:
        if total_missing == 0:
            print(f'{GREEN}All tests passed{RESET} in {total_duration:.3f} seconds')
        else:
            print(f'{YELLOW}All tests passed{RESET} in {total_duration:.3f} seconds, missing {total_missing} cases')
    else:
        print(f'{RED}{pass_count} out of {test_count} tests passed{RESET}, in {total_duration:.3f} seconds')
    
if __name__ == '__main__':
    main()