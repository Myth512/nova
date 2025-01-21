missing = 0

# Test tuple creation
t1 = (1, 2, 3)
assert t1 == (1, 2, 3)

t2 = (4,)
assert t2 == (4,)
assert len(t2) == 1

t3 = tuple()
assert t3 == ()

t4 = (1, 2, 3, 4, 5)
assert len(t4) == 5

# Test tuple indexing
assert t1[0] == 1
assert t1[1] == 2
assert t1[2] == 3

# Test tuple negative indexing
assert t1[-1] == 3
assert t1[-2] == 2
assert t1[-3] == 1

# Test tuple slicing
missing += 1
# assert t1[1:3] == (2, 3)
# assert t1[:2] == (1, 2)
# assert t1[3:] == (4, 5)
# assert t1[::2] == (1, 3, 5)

# Test concatenation of tuples
t5 = (6, 7, 8)
assert t1 + t5 == (1, 2, 3, 6, 7, 8)

# Test repetition of tuples
t6 = (9, 10)
assert t6 * 2 == (9, 10, 9, 10)
assert t6 * 3 == (9, 10, 9, 10, 9, 10)

# Test checking if element is in tuple
assert 1 in t1
assert 4 not in t1

# Test tuple length
assert len(t1) == 3
assert len(t5) == 3

# Test tuple iteration
t7 = (1, 2, 3)
elements = []
for element in t7:
    elements.append(element)
assert elements == [1, 2, 3]

# Test tuple comparison
assert t1 < t5
assert t1 <= t5
assert t5 > t1
assert t5 >= t1
assert t1 == (1, 2, 3)
assert t1 != (2, 3, 4)

# Test nested tuples
t8 = ((1, 2), (3, 4), (5, 6))
assert t8[0] == (1, 2)
assert t8[1][0] == 3

# Test tuple unpacking
a, b, c = t1
assert a == 1
assert b == 2
assert c == 3

# Test tuple unpacking with nested tuples
missing += 1
# (t9a, t9b), (t10a, t10b) = ((1, 2), (3, 4))
# assert t9a == 1
# assert t9b == 2
# assert t10a == 3
# assert t10b == 4

# Test tuple unpacking in loop
missing += 1
# for a, b in [(1, 2), (3, 4)]:
#     assert a + b == 3 or a + b == 7

# Test tuple unpacking with different lengths (invalid, should raise error)
missing += 1
# try:
#     a, b = (1, 2, 3)
#     assert False, "Expected ValueError"
# except ValueError:
#     pass

# Test tuple with different data types
t9 = (1, "hello", 3.14, True)
assert t9[0] == 1
assert t9[1] == "hello"
assert t9[2] == 3.14
assert t9[3] is True

# Test nested tuple unpacking
missing += 1
# t10 = ((1, 2), (3, 4))
# (a, b), (c, d) = t10
# assert a == 1
# assert b == 2
# assert c == 3
# assert d == 4

# Test using tuple as dictionary key
missing += 1
# t11 = (1, 2)
# t12 = (3, 4)
# d = {t11: "value1", t12: "value2"}
# assert d[t11] == "value1"
# assert d[t12] == "value2"

# Test tuple with list (immutable vs mutable)
t13 = (1, 2, 3)
t14 = [4, 5, 6]
t15 = t13 + tuple(t14)
assert t15 == (1, 2, 3, 4, 5, 6)

# Test tuple with empty list
t16 = (1, 2, 3)
t17 = []
t18 = t16 + tuple(t17)
assert t18 == (1, 2, 3)

# Test slicing with steps (positive step)
missing += 1
# t19 = (1, 2, 3, 4, 5)
# assert t19[::2] == (1, 3, 5)

# Test slicing with negative step
missing += 1
# assert t19[::-1] == (5, 4, 3, 2, 1)

# Test repeated elements in tuple
t20 = (1, 1, 1)
assert t20 == (1, 1, 1)
assert t20 * 3 == (1, 1, 1, 1, 1, 1, 1, 1, 1)

# Test immutable nature of tuple (raising errors on mutation)
try:
    t1[0] = 100
    assert False, "Expected TypeError"
except TypeError:
    pass

# Test tuple of tuples
t21 = ((1, 2), (3, 4), (5, 6))
assert t21[0] == (1, 2)
assert t21[1] == (3, 4)
assert t21[2] == (5, 6)

# Test creating tuple with from an iterable
t22 = tuple([7, 8, 9])
assert t22 == (7, 8, 9)

# Test large tuple (just for performance testing, size not directly relevant)
t23 = tuple(range(1000))
assert len(t23) == 1000

# Test tuple slicing with large index
missing += 1
# assert t19[100:] == ()

# Test creating tuple from range (not allowed in your interpreter)
assert tuple(range(5)) == (0, 1, 2, 3, 4)

# Test tuple with None
t24 = (None, None)
assert t24 == (None, None)

# Test slicing with a step size of 1
missing += 1
# t25 = (10, 20, 30, 40, 50)
# assert t25[1:4:1] == (20, 30, 40)

# Test changing a mutable element inside a tuple (unsupported in your interpreter)
t26 = ([], [], [])
t26[0].append(100)
assert t26[0] == [100]  # Uncomment when mutable objects inside tuple are supported

# Test creation of tuple with a mix of immutable and mutable objects
t27 = (1, [2, 3], "hello")
t27[1].append(4)  # Lists are mutable, so this is allowed
assert t27 == (1, [2, 3, 4], "hello")

# Initialize the tuple
tpl = (1, 2, 3, 2, 4, 2, 5)

# Test counting an element that exists multiple times
assert tpl.count(2) == 3, f"Expected 3, but got {tpl.count(2)}"

# Test counting an element that exists once
assert tpl.count(4) == 1, f"Expected 1, but got {tpl.count(4)}"

# Test counting an element that does not exist
assert tpl.count(99) == 0, f"Expected 0, but got {tpl.count(99)}"

# Test counting with different data types
tpl = (1, "apple", 2, "banana", "apple", 3)
assert tpl.count("apple") == 2, f"Expected 2, but got {tpl.count('apple')}"
assert tpl.count(1) == 1, f"Expected 1, but got {tpl.count(1)}"
assert tpl.count(3) == 1, f"Expected 1, but got {tpl.count(3)}"

# Test counting in an empty tuple
tpl = ()
assert tpl.count(1) == 0, f"Expected 0, but got {tpl.count(1)}"

# Initialize the tuple
tpl = (1, 2, 3, 2, 4, 2, 5)

# Test finding the first occurrence of an element
assert tpl.index(2) == 1, f"Expected 1, but got {tpl.index(2)}"

# Test finding an element with start and stop
assert tpl.index(2, 2) == 3, f"Expected 3, but got {tpl.index(2, 2)}"
assert tpl.index(2, 2, 4) == 3, f"Expected 3, but got {tpl.index(2, 2, 4)}"

# Test finding the first occurrence of an element starting at 0
assert tpl.index(1) == 0, f"Expected 0, but got {tpl.index(1)}"

# Test finding an element at the end of the tuple
assert tpl.index(5) == 6, f"Expected 6, but got {tpl.index(5)}"

# Test finding an element not in the tuple
try:
    tpl.index(99)
    assert False, "Expected a ValueError, but no exception was raised"
except ValueError:
    pass  # Expected behavior

# Test with mixed data types
tpl = (1, "apple", 2, "banana", "apple", 3)
assert tpl.index("apple") == 1, f"Expected 1, but got {tpl.index('apple')}"
assert tpl.index(3) == 5, f"Expected 5, but got {tpl.index(3)}"

# Test start and stop with mixed data types
assert tpl.index("apple", 2) == 4, f"Expected 4, but got {tpl.index('apple', 2)}"

# Test with a single-element tuple
tpl = (42,)
assert tpl.index(42) == 0, f"Expected 0, but got {tpl.index(42)}"

# Test with an empty tuple
tpl = ()
try:
    tpl.index(1)
    assert False, "Expected a ValueError, but no exception was raised"
except ValueError:
    pass  # Expected behavior

print(f'missing: {missing}')