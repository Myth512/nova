missing = 0

# Test simple iteration through a list
for i in [0, 1, 2, 3, 4]:
    assert i in [0, 1, 2, 3, 4]

# Test iteration through an empty list
for i in []:
    assert False, "This block should not execute for an empty list"

# Test loop with an else block that executes
for i in [1, 2, 3]:
    pass
else:
    success = True
assert success

# Test loop with an else block that does not execute due to a break
for i in [1, 2, 3]:
    if i == 2:
        break
else:
    assert False, "Else block should not execute when loop is broken"

result = []

for i in range(3):
    for j in range(3):
        result.append((i, j))
assert result == [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1), (1, 2), (2, 0), (2, 1), (2, 2)]

# Test nested loops with an else block
outer_success = False
inner_success = False
for i in [1, 2]:
    for j in [3, 4]:
        if j == 3:
            break
    else:
        inner_success = True  # Else block in inner loop
else:
    outer_success = True  # Else block in outer loop
assert not inner_success
assert outer_success

# Test breaking out of a loop
sum = 0
for i in [1, 2, 3, 4, 5]:
    if i == 4:
        break
    sum += i
assert sum == 6

# Test continuing within a loop
result = []
for i in [1, 2, 3, 4, 5]:
    if i % 2 == 0:
        continue
    result.append(i)
assert result == [1, 3, 5]

# Test iteration over a string
chars = []
for char in "hello":
    chars.append(char)
else:
    chars.append("done")
assert chars == ['h', 'e', 'l', 'l', 'o', 'done']

# Test iteration over a tuple
for item in (1, 2, 3):
    assert item in (1, 2, 3)

# Test iteration over a nested list
nested = [[1, 2], [3, 4], [5, 6]]
flattened = []
for sublist in nested:
    for item in sublist:
        flattened.append(item)
assert flattened == [1, 2, 3, 4, 5, 6]

# Test loop with no body
for _ in [1, 2, 3]:
    pass  # Ensure no errors occur when loop body is empty

# Test modifying a list inside a loop
data = [1, 2, 3, 4, 5]
for i in range(len(data)):
    data[i] *= 2
assert data == [2, 4, 6, 8, 10]

# Test breaking out of multiple nested loops
broken = False
for i in [1, 2, 3]:
    for j in [4, 5, 6]:
        if i == 2 and j == 5:
            broken = True
            break
    if broken:
        break
assert broken

# Test reverse iteration using slicing
reverse = []
for i in [1, 2, 3][::-1]:
    reverse.append(i)
assert reverse == [3, 2, 1]

# Deeply nested loop
s = 0
for i in range(10):
    for j in range(10):
        for k in range(10):
            for w in range(10):
                s += 1
assert s == 10000

# Test edge case with empty iterable and else block
for i in []:
    assert False, "This should not execute"
else:
    empty_else = True
assert empty_else

# Test iterating over a range with negative steps
reverse_range = []
for i in range(10, 0, -2):
    reverse_range.append(i)
else:
    reverse_range.append("done")
assert reverse_range == [10, 8, 6, 4, 2, "done"]

# Test loop variable scoping after the loop
for i in [1, 2, 3]:
    pass
assert i == 3  # Variable `i` should persist after the loop

# Test iterating over a range with zero-length
for i in range(0):
    assert False, "This block should not execute for an empty range"
else:
    zero_range_else = True
assert zero_range_else

# Test list comprehension with nested loop behavior
missing += 1
# squares = []
# for i in [1, 2, 3]:
#     squares.append([x ** 2 for x in range(1, i + 1)])
# else:
#     squares.append("done")
# assert squares == [[1], [1, 4], [1, 4, 9], "done"]

print(f'missing: {missing}')