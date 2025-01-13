# Basic while loop
a = 0
while a < 5:
    a += 1
assert a == 5  # Loop executed 5 times, a should be 5

# While loop with break
a = 0
while a < 10:
    if a == 5:
        break
    a += 1
assert a == 5  # Loop exited early with break when a == 5

# While loop with continue
# a = 0
# result = []
# while a < 5:
    # a += 1
    # if a == 3:
        # continue
    # result.append(a)
# assert result == [1, 2, 4, 5]  # Loop skipped adding 3 due to continue

# While loop with multiple conditions
a = 0
b = 5
while a < 5 and b > 0:
    a += 1
    b -= 1
assert a == 5 and b == 0  # a increments while b decrements

# Nested while loop
a = 0
b = 0
while a < 3:
    while b < 2:
        b += 1
    a += 1
assert a == 3 and b == 2  # Inner loop runs twice for each iteration of the outer loop

# Infinite while loop with break (manually breaking)
a = 0
while True:
    a += 1
    if a == 10:
        break
assert a == 10  # Infinite loop stopped manually when a == 10

# While loop with condition based on a collection
# a = [1, 2, 3, 4, 5]
# i = 0
# result = []
# while i < len(a):
#     result.append(a[i])
#     i += 1
# assert result == [1, 2, 3, 4, 5]  # Loop iterates over the list

# While loop with a decrementing counter
a = 10
while a > 0:
    a -= 1
assert a == 0  # a decrements to 0

# Complex condition with multiple updates inside the loop
a = 0
b = 10
while a < 5 and b > 0:
    a += 1
    b -= 2
assert a == 5 and b == 0  # a increments and b decrements by 2 each time

# While loop with non-numeric type (using list and index)
# a = ['apple', 'banana', 'cherry']
# i = 0
# result = []
# while i < len(a):
#     result.append(a[i])
#     i += 1
# assert result == ['apple', 'banana', 'cherry']  # Loop iterates over the list of strings

# While loop with complex logic and break/continue
a = 0
b = 0
while a < 10:
    if a == 5:
        a += 1
        continue  # Skip this iteration when a == 5
    if b == 3:
        break  # Break the loop when b == 3
    a += 1
    b += 1
assert a == 3 and b == 3  # Loop breaks when b reaches 3 and skips iteration when a == 5

# Infinite loop with an external condition
a = 0
while a < 10:
    a += 1
    if a == 10:
        break
assert a == 10  # The loop breaks when a reaches 10

# While loop with a large number of iterations
a = 0
while a < 1000000:
    a += 1
assert a == 1000000  # The loop iterates one million times

# While loop that performs calculations (mix of operations)
a = 0
b = 1
while a < 5:
    b *= 2  # Double b on each iteration
    a += 1
assert b == 32  # b = 1 * 2^5, after 5 iterations

# While loop with logical and arithmetic operations
a = 10
b = 0
while a > 0:
    b += a % 3  # Add the remainder of a divided by 3 to b
    a -= 1
assert b == 10  # Sum of remainders from a % 3 when a = 10, 9, 8, ..., 1
