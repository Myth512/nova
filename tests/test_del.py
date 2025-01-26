missing = 0

# Test: Delete a simple variable
x = 10
del x
try:
    x
    assert False, "Variable x should be deleted"
except NameError:
    pass

# Test: Delete multiple variables at once
a, b, c = 1, 2, 3
del a, b
try:
    a
    assert False, "Variable a should be deleted"
except NameError:
    pass

try:
    b
    assert False, "Variable b should be deleted"
except NameError:
    pass

assert c == 3  # Variable c should remain intact

# Test: Delete an item from a list
lst = [1, 2, 3, 4]
del lst[1]
assert lst == [1, 3, 4]

# Test: Delete a slice from a list
missing += 1
# lst = [1, 2, 3, 4, 5]
# del lst[1:4]
# assert lst == [1, 5]

# Test: Delete all items from a list
missing += 1
# lst = [1, 2, 3, 4, 5]
# del lst[:]
# assert lst == []

# Test: Delete a dictionary key
d = {"a": 1, "b": 2, "c": 3}
del d["b"]
assert d == {"a": 1, "c": 3}

# Test: Delete a variable and reassign
x = 42
del x
x = 99
assert x == 99

# Test: Delete an element from a nested list
nested_list = [[1, 2], [3, 4], [5, 6]]
del nested_list[1][0]
assert nested_list == [[1, 2], [4], [5, 6]]

# Test: Delete a dictionary key inside a nested dictionary
nested_dict = {"outer": {"inner": 42}, "other": 99}
del nested_dict["outer"]["inner"]
assert nested_dict == {"outer": {}, "other": 99}

# Test: Delete an attribute from an object
class Test:
    def __init__(self):
        self.x = 10

obj = Test()
del obj.x
try:
    obj.x
    assert False, "Attribute x should be deleted"
except AttributeError:
    pass

# Test: Delete an item from a list inside a dictionary
complex_structure = {"numbers": [10, 20, 30], "letters": ["a", "b", "c"]}
del complex_structure["numbers"][1]
assert complex_structure == {"numbers": [10, 30], "letters": ["a", "b", "c"]}

# Test: Delete a key from a dictionary inside a list
complex_structure = [{"a": 1, "b": 2}, {"c": 3, "d": 4}]
del complex_structure[1]["d"]
assert complex_structure == [{"a": 1, "b": 2}, {"c": 3}]

# Test: Delete a variable in the global scope
global_var = 100

def delete_global():
    global global_var
    del global_var

delete_global()
try:
    global_var
    assert False, "Global variable should be deleted"
except NameError:
    pass

# Test: Delete a variable inside a closure
missing += 1
# def outer():
#     x = 10

#     def inner():
#         nonlocal x
#         del x
#     inner()
#     try:
#         x
#         assert False, "Variable x in closure should be deleted"
#     except UnboundLocalError:
#         pass

# outer()

# Test: Delete a key that doesn't exist in a dictionary
d = {"a": 1, "b": 2}
try:
    del d["c"]
    assert False, "KeyError should be raised for missing key"
except KeyError:
    pass

# Test: Delete an index that doesn't exist in a list
lst = [1, 2, 3]
try:
    del lst[10]
    assert False, "IndexError should be raised for out-of-range index"
except IndexError:
    pass

# Test: Delete an element using a negative index
lst = [1, 2, 3]
del lst[-1]
assert lst == [1, 2]

# Test: Delete an empty list
lst = []
try:
    del lst[0]
    assert False, "IndexError should be raised for deletion from an empty list"
except IndexError:
    pass

# Test: Delete a slice with a step
missing += 1
# lst = [0, 1, 2, 3, 4, 5, 6]
# del lst[::2]
# assert lst == [1, 3, 5]

# Test: Delete an entire list using del
lst = [1, 2, 3]
del lst
try:
    lst
    assert False, "List should be deleted"
except NameError:
    pass

# Test: Delete a variable repeatedly
x = 5
del x
try:
    del x
    assert False, "NameError should be raised for deleting an undefined variable"
except NameError:
    pass

print(f'missing: {missing}')