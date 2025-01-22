missing = 0

# Initialize a dictionary
d = {"a": 1, "b": 2, "c": 3}

# Test dictionary creation
assert isinstance(d, dict), "d is not a dictionary"

assert type(d) is dict

# Test item access
assert d["a"] == 1, f"Expected 1, but got {d['a']}"
assert d["b"] == 2, f"Expected 2, but got {d['b']}"

# Test item assignment
d["a"] = 10
assert d["a"] == 10, f"Expected 10, but got {d['a']}"

# Test adding new keys
d["d"] = 4
assert d["d"] == 4, f"Expected 4, but got {d['d']}"

# Test deleting a key
del d["d"]
assert "d" not in d, "Key 'd' was not deleted"

# Test `in` operator
assert "a" in d, "Key 'a' should be in the dictionary"
assert "z" not in d, "Key 'z' should not be in the dictionary"

# Test `get` method
assert d.get("a") == 10, f"Expected 10, but got {d.get('a')}"
assert d.get("z") is None, f"Expected None, but got {d.get('z')}"
assert d.get("z", 0) == 0, f"Expected 0, but got {d.get('z', 0)}"

# Test `keys` method (order matters)
assert list(d.keys()) == ["a", "b", "c"], f"Expected ['a', 'b', 'c'], but got {d.keys()}"

# Test `values` method (order matters)
assert list(d.values()) == [10, 2, 3], f"Expected [10, 2, 3], but got {d.values()}"

# Test `items` method (order matters)
assert list(d.items()) == [("a", 10), ("b", 2), ("c", 3)], f"Expected [('a', 10), ('b', 2), ('c', 3)], but got {d.items()}"

# Test `pop` method
value = d.pop("a")
assert value == 10, f"Expected 10, but got {value}"
assert "a" not in d, "Key 'a' was not removed after pop"

# Test `pop` with default
value = d.pop("z", "default")
assert value == "default", f"Expected 'default', but got {value}"

# Test `popitem` method (LIFO behavior)
d = {"a": 1, "b": 2, "c": 3}
key, value = d.popitem()
assert (key, value) == ("c", 3), f"Expected ('c', 3), but got ({key}, {value})"
assert list(d.items()) == [("a", 1), ("b", 2)], f"Expected [('a', 1), ('b', 2)], but got {d.items()}"

# Test `update` method with overlapping keys
d.update({"b": 20, "d": 4})
assert d == {"a": 1, "b": 20, "d": 4}, f"Expected {{'a': 1, 'b': 20, 'd': 4}}, but got {d}"

# Test `update` with no overlap
d.update({"e": 5})
assert d == {"a": 1, "b": 20, "d": 4, "e": 5}, f"Expected {{'a': 1, 'b': 20, 'd': 4, 'e': 5}}, but got {d}"

# Test `clear` method
d.clear()
assert d == {}, f"Expected an empty dictionary, but got {d}"

# Test `fromkeys` with no default value
missing += 1
# keys = ["x", "y", "z"]
# d = dict.fromkeys(keys)
# assert d == {"x": None, "y": None, "z": None}, f"Expected {{'x': None, 'y': None, 'z': None}}, but got {d}"

# Test `fromkeys` with a default value
missing += 1
# d = dict.fromkeys(keys, 0)
# assert d == {"x": 0, "y": 0, "z": 0}, f"Expected {{'x': 0, 'y': 0, 'z': 0}}, but got {d}"

# Test `copy` method
missing += 1
# d = {"a": 1, "b": 2}
# d_copy = d.copy()
# assert d_copy == d, f"Expected {d}, but got {d_copy}"
# assert d_copy is not d, "d_copy should be a separate instance"

# Test `setdefault` with existing key
d = {"a": 1, "b": 2}
value = d.setdefault("a", 10)
assert value == 1, f"Expected 1, but got {value}"
assert d == {"a": 1, "b": 2}, f"Expected {{'a': 1, 'b': 2}}, but got {d}"

# Test `setdefault` with new key
value = d.setdefault("c", 3)
assert value == 3, f"Expected 3, but got {value}"
assert d == {"a": 1, "b": 2, "c": 3}, f"Expected {{'a': 1, 'b': 2, 'c': 3}}, but got {d}"

# Test dictionary comparisons (equality and order-insensitive keys)
d1 = {"a": 1, "b": 2}
d2 = {"b": 2, "a": 1}
assert d1 == d2, "Dictionaries with the same items should be equal"
assert d1 != {"a": 1}, "Dictionaries with different items should not be equal"

# Test nested dictionaries
nested = {"outer": {"inner": 1}}
assert nested["outer"]["inner"] == 1, f"Expected 1, but got {nested['outer']['inner']}"

# Test dictionary with mixed data types
missing += 1
# mixed = {1: "integer key", (2, 3): "tuple key"}
# assert mixed[1] == "integer key", f"Expected 'integer key', but got {mixed[1]}"
# assert mixed[(2, 3)] == "tuple key", f"Expected 'tuple key', but got {mixed[(2, 3)]}"

print(f'missing: {missing}')