missing = 0

### Test initialization
# Basic range
r = range(5)
assert list(r) == [0, 1, 2, 3, 4], f"Expected [0, 1, 2, 3, 4], but got {list(r)}"

# Range with start and stop
r = range(1, 6)
assert list(r) == [1, 2, 3, 4, 5], f"Expected [1, 2, 3, 4, 5], but got {list(r)}"

# Range with start, stop, and step
r = range(1, 10, 2)
assert list(r) == [1, 3, 5, 7, 9], f"Expected [1, 3, 5, 7, 9], but got {list(r)}"

# Negative step
r = range(10, 0, -2)
assert list(r) == [10, 8, 6, 4, 2], f"Expected [10, 8, 6, 4, 2], but got {list(r)}"

# Empty range
r = range(5, 5)
assert list(r) == [], f"Expected [], but got {list(r)}"

r = range(0)
assert list(r) == [], f"Expected [], but got {list(r)}"

# Edge cases for start, stop, and step
r = range(0, 0)
assert list(r) == [], f"Expected [], but got {list(r)}"

r = range(0, -5)
assert list(r) == [], f"Expected [], but got {list(r)}"

r = range(10, 10)
assert list(r) == [], f"Expected [], but got {list(r)}"

### Test indexing
r = range(10)
assert r[0] == 0, f"Expected 0, but got {r[0]}"
assert r[5] == 5, f"Expected 5, but got {r[5]}"
assert r[-1] == 9, f"Expected 9, but got {r[-1]}"
assert r[-5] == 5, f"Expected 5, but got {r[-5]}"

# Index out of range
try:
    _ = r[10]
    assert False, "Expected IndexError, but no exception was raised"
except IndexError:
    pass

try:
    _ = r[-11]
    assert False, "Expected IndexError, but no exception was raised"
except IndexError:
    pass

### Test slicing
missing += 1
# r = range(10)
# assert list(r[:5]) == [0, 1, 2, 3, 4], f"Expected [0, 1, 2, 3, 4], but got {list(r[:5])}"
# assert list(r[5:]) == [5, 6, 7, 8, 9], f"Expected [5, 6, 7, 8, 9], but got {list(r[5:])}"
# assert list(r[::2]) == [0, 2, 4, 6, 8], f"Expected [0, 2, 4, 6, 8], but got {list(r[::2])}"
# assert list(r[::-1]) == [9, 8, 7, 6, 5, 4, 3, 2, 1, 0], f"Expected reversed range, but got {list(r[::-1])}"

# # Negative step in slicing
# assert list(r[-1:-6:-1]) == [9, 8, 7, 6, 5], f"Expected [9, 8, 7, 6, 5], but got {list(r[-1:-6:-1])}"
# assert list(r[-1::-2]) == [9, 7, 5, 3, 1], f"Expected [9, 7, 5, 3, 1], but got {list(r[-1::-2])}"

# # Empty slices
# assert list(r[10:]) == [], f"Expected [], but got {list(r[10:])}"

### Test `in` and `not in`
r = range(10)
assert 5 in r, "Expected 5 to be in range(10)"
assert 15 not in r, "Expected 15 not to be in range(10)"
assert -1 not in r, "Expected -1 not to be in range(10)"

### Test len()
r = range(10)
assert len(r) == 10, f"Expected len(r) = 10, but got {len(r)}"

r = range(1, 10, 2)
assert len(r) == 5, f"Expected len(r) = 5, but got {len(r)}"

r = range(10, 0, -2)
assert len(r) == 5, f"Expected len(r) = 5, but got {len(r)}"

r = range(0, 0)
assert len(r) == 0, f"Expected len(r) = 0, but got {len(r)}"

### Test equality
r1 = range(0, 10, 2)
r2 = range(0, 10, 2)
r3 = range(1, 10, 2)
assert r1 == r2, "Expected ranges to be equal"
assert r1 != r3, "Expected ranges to be not equal"

### Test iteration
r = range(5)
values = []
for i in r:
    values.append(i)
assert values == [0, 1, 2, 3, 4], f"Expected [0, 1, 2, 3, 4], but got {values}"

### Test extreme ranges
# Very large range
r = range(0, 10**6, 10**5)
assert len(r) == 10, f"Expected len(r) = 10, but got {len(r)}"
assert r[0] == 0, f"Expected 0, but got {r[0]}"
assert r[-1] == 900000, f"Expected 900000, but got {r[-1]}"

## Test invalid ranges
try:
    r = range(0, 10, 0)  # Step cannot be 0
    assert False, "Expected ValueError, but no exception was raised"
except ValueError:
    pass

### Custom cases
# Reverse step with bounds
r = range(5, -5, -1)
assert list(r) == [5, 4, 3, 2, 1, 0, -1, -2, -3, -4], f"Expected [5, 4, 3, 2, 1, 0, -1, -2, -3, -4], but got {list(r)}"

# Negative bounds with positive step
r = range(-5, 5, 2)
assert list(r) == [-5, -3, -1, 1, 3], f"Expected [-5, -3, -1, 1, 3], but got {list(r)}"

# Test iterating over range objects

# Simple iteration over range(stop)
r = range(5)
result = []
for i in r:
    result.append(i)
assert result == [0, 1, 2, 3, 4], f"Expected [0, 1, 2, 3, 4], but got {result}"

# Iterating over range(start, stop)
r = range(2, 7)
result = []
for i in r:
    result.append(i)
assert result == [2, 3, 4, 5, 6], f"Expected [2, 3, 4, 5, 6], but got {result}"

# Iterating over range(start, stop, step)
r = range(1, 10, 3)
result = []
for i in r:
    result.append(i)
assert result == [1, 4, 7], f"Expected [1, 4, 7], but got {result}"

# Iterating over range with negative step
r = range(10, 0, -2)
result = []
for i in r:
    result.append(i)
assert result == [10, 8, 6, 4, 2], f"Expected [10, 8, 6, 4, 2], but got {result}"

# Iterating over empty range
r = range(5, 5)
result = []
for i in r:
    result.append(i)
assert result == [], f"Expected [], but got {result}"

# Iterating over a single-element range
r = range(5, 6)
result = []
for i in r:
    result.append(i)
assert result == [5], f"Expected [5], but got {result}"

# Iterating over reversed range
r = range(5, -1, -1)
result = []
for i in r:
    result.append(i)
assert result == [5, 4, 3, 2, 1, 0], f"Expected [5, 4, 3, 2, 1, 0], but got {result}"

# Iterating over range with large values
r = range(0, 1000000, 100000)
result = []
for i in r:
    result.append(i)
assert result == [0, 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000], \
    f"Expected [0, 100000, 200000, ..., 900000], but got {result}"

# Nested iteration over range
r1 = range(3)
r2 = range(2)
result = []
for i in r1:
    for j in r2:
        result.append((i, j))
assert result == [(0, 0), (0, 1), (1, 0), (1, 1), (2, 0), (2, 1)], \
    f"Expected [(0, 0), (0, 1), ..., (2, 1)], but got {result}"

# Using range with comprehension
missing += 1
# r = range(4)
# result = [x ** 2 for x in r]
# assert result == [0, 1, 4, 9], f"Expected [0, 1, 4, 9], but got {result}"

# Using range with `enumerate`
missing += 1
# r = range(5)
# result = []
# for idx, val in enumerate(r):
    # result.append((idx, val))
# assert result == [(0, 0), (1, 1), (2, 2), (3, 3), (4, 4)], \
    # f"Expected [(0, 0), (1, 1), ..., (4, 4)], but got {result}"

# Iterating over range with odd bounds
r = range(-3, 4)
result = []
for i in r:
    result.append(i)
assert result == [-3, -2, -1, 0, 1, 2, 3], f"Expected [-3, -2, -1, 0, 1, 2, 3], but got {result}"

# Iterating backwards with slicing
missing += 1
# r = range(10)
# result = []
# for i in r[::-1]:
#     result.append(i)
# assert result == [9, 8, 7, 6, 5, 4, 3, 2, 1, 0], f"Expected [9, 8, 7, ..., 0], but got {result}"

# Iterating with mixed step sizes
r = range(0, 10, 3)
result = []
for i in r:
    result.append(i)
assert result == [0, 3, 6, 9], f"Expected [0, 3, 6, 9], but got {result}"

# Test methods and attributes of the range class

# Test basic attributes: start, stop, and step
r = range(1, 10, 2)
assert r.start == 1, f"Expected start=1, but got {r.start}"
assert r.stop == 10, f"Expected stop=10, but got {r.stop}"
assert r.step == 2, f"Expected step=2, but got {r.step}"

# Test default values of start and step
r = range(5)
assert r.start == 0, f"Expected start=0, but got {r.start}"
assert r.stop == 5, f"Expected stop=5, but got {r.stop}"
assert r.step == 1, f"Expected step=1, but got {r.step}"

# Test equality of ranges with identical attributes
r1 = range(0, 10, 2)
r2 = range(0, 10, 2)
assert r1 == r2, "Expected r1 and r2 to be equal, but they are not"

# Test inequality of ranges with different attributes
r3 = range(1, 10, 2)
assert r1 != r3, "Expected r1 and r3 to be not equal, but they are equal"

# Test membership using `in` and `not in`
r = range(0, 10, 2)
assert 4 in r, "Expected 4 to be in range(0, 10, 2)"
assert 5 not in r, "Expected 5 not to be in range(0, 10, 2)"

# Test slicing attributes
missing += 1
# r = range(10)
# sliced = r[2:8:2]
# assert sliced.start == 2, f"Expected sliced.start=2, but got {sliced.start}"

print(f'missing: {missing}')