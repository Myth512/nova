missing = 0

# Strings
assert len("") == 0
assert len("a") == 1
assert len("hello") == 5
assert len(" ") == 1

# Lists
assert len([]) == 0
assert len([1]) == 1
assert len([1, 2, 3]) == 3
assert len([[1, 2], [3, 4]]) == 2

# Tuples
assert len(()) == 0
assert len((1,)) == 1
assert len((1, 2, 3)) == 3
assert len(((1, 2), (3, 4))) == 2

# Sets
missing += 1
# assert len(set()) == 0
# assert len({1}) == 1
# assert len({1, 2, 3}) == 3
# assert len({(1, 2), (3, 4)}) == 2

# Dictionaries
assert len({}) == 0
assert len({"a": 1}) == 1
assert len({"a": 1, "b": 2, "c": 3}) == 3
assert len({1: "a", 2: "b"}) == 2

# Ranges
assert len(range(0)) == 0
assert len(range(5)) == 5
assert len(range(1, 10, 2)) == 5

print(f'missing: {missing}')