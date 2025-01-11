import subprocess
import os
import time

RED = '\033[31m'
GREEN = '\033[32m'
RESET = '\033[0m'

TEST_DIR = 'tests'
INTERPRETER = 'nova'

def run_test(name: str) -> int:
    script_path = os.path.join(TEST_DIR, name)

    start_time = time.time()

    result = subprocess.run([INTERPRETER, script_path])

    end_time = time.time()

    duration = end_time - start_time

    return result.returncode, duration

def main():
    tests = [f for f in os.listdir(TEST_DIR) if f.startswith('test_')]
    test_count = 0
    pass_count = 0
    total_duration = 0

    for test in tests:
        test_count += 1
        returncode, duration = run_test(test)
        total_duration += duration

        if returncode == 0: 
            pass_count += 1
            print(f'{GREEN}[✓]{RESET} {test} passed, in {duration:.3f} seconds')
        else:
            print(f'{RED}[✗]{RESET} {test} failed, in {duration:.3f} seconds')
        
    if pass_count == test_count:
        print(f'{GREEN}All tests passed{RESET} in {total_duration:.3f} seconds')
    else:
        print(f'{RED}{pass_count} out of {test_count} tests passed{RESET}, in {total_duration:.3f} seconds')
    
if __name__ == '__main__':
    main()