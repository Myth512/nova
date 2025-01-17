missing = 0

# Test list creation
lst = []
assert lst == []

lst = [1, 2, 3, 4]
assert lst == [1, 2, 3, 4]

# Test list index access
lst = [10, 20, 30, 40]
assert lst[0] == 10
assert lst[1] == 20
assert lst[2] == 30
assert lst[3] == 40

# Test negative index access
assert lst[-1] == 40
assert lst[-2] == 30
assert lst[-3] == 20
assert lst[-4] == 10

# Test slicing
missing += 1
# lst = [1, 2, 3, 4, 5, 6, 7]
# assert lst[2:5] == [3, 4, 5]
# assert lst[:3] == [1, 2, 3]
# assert lst[3:] == [4, 5, 6, 7]
# assert lst[::2] == [1, 3, 5, 7]
# assert lst[::-1] == [7, 6, 5, 4, 3, 2, 1]
# assert lst[-3:] == [5, 6, 7]
# assert lst[:-2] == [1, 2, 3, 4, 5]

# Test list length
lst = [1, 2, 3]
assert len(lst) == 3
lst.append(4)
assert len(lst) == 4
lst.pop()
assert len(lst) == 3

# Test append method
lst = [1, 2, 3]
lst.append(4)
assert lst == [1, 2, 3, 4]

# Test insert method
missing += 1 
# lst.insert(1, 5)  # Insert 5 at index 1
# assert lst == [1, 5, 2, 3, 4]

# Test extend method
missing += 1
# lst.extend([6, 7])
# assert lst == [1, 5, 2, 3, 4, 6, 7]

# Test pop method
lst = [1, 5, 2, 3, 4, 6, 7]
popped_item = lst.pop()
assert popped_item == 7
assert lst == [1, 5, 2, 3, 4, 6]

# Test pop with index
missing += 1
# popped_item = lst.pop(2)
# assert popped_item == 2
# assert lst == [1, 5, 3, 4, 6]

# Test remove method
missing += 1
# lst.remove(5)
# assert lst == [1, 3, 4, 6]

# Test clear method
missing += 1
# lst.clear()
# assert lst == []

# Test index method
missing += 1
# lst = [1, 2, 3, 4, 5]
# assert lst.index(3) == 2

# Test count method
missing += 1
# lst = [1, 2, 2, 3, 4, 5, 2]
# assert lst.count(2) == 3
# assert lst.count(6) == 0

# Test copy method
missing += 1
# lst = [1, 2, 3]
# lst_copy = lst.copy()
# assert lst_copy == [1, 2, 3]
# assert lst is not lst_copy  # Ensure it's a new list

# Test reverse method
missing += 1
# lst = [1, 2, 3]
# lst.reverse()
# assert lst == [3, 2, 1]

# Test sort method
missing += 1 
# lst = [3, 1, 4, 2]
# lst.sort()
# assert lst == [1, 2, 3, 4]

# Test reverse sort
missing += 1
# lst.sort(reverse=True)
# assert lst == [4, 3, 2, 1]

# Test concatenation (+)
lst1 = [1, 2]
lst2 = [3, 4]
assert lst1 + lst2 == [1, 2, 3, 4]

# Test repetition (*)
assert lst1 * 3 == [1, 2, 1, 2, 1, 2]

# Test equality (==)
assert [1, 2, 3] == [1, 2, 3]
assert [1, 2, 3] != [4, 5, 6]

# Test inequality (!=)
assert [1, 2] != [2, 3]

# Test greater than (>)
lst1 = [1, 2, 3]
lst2 = [0, 1, 2]
assert lst1 > lst2
assert not lst2 > lst1

# Test greater than or equal to (>=)
assert lst1 >= lst2
assert lst1 >= [1, 2, 3]

# Test less than (<)
assert lst2 < lst1
assert not lst1 < lst2

# Test less than or equal to (<=)
assert lst2 <= lst1
assert lst1 <= [1, 2, 3]

# Test membership (in)
assert 3 in [1, 2, 3]
assert 5 not in [1, 2, 3]

# Test list comprehension
missing += 1
# lst = [x * 2 for x in range(5)]
# assert lst == [0, 2, 4, 6, 8]

# Test nested lists
nested_lst = [[1, 2], [3, 4]]
assert nested_lst[0] == [1, 2]
assert nested_lst[1] == [3, 4]
assert nested_lst[0][1] == 2

# Test boolean conversion
assert bool([]) == False
assert bool([1]) == True

# Test list unpacking
lst = [1, 2, 3]
a, b, c = lst
assert a == 1
assert b == 2
assert c == 3

# Test tuple assignment with lists
lst = [1, 2, 3]
t = (4, 5, 6)
lst, t = t, lst
assert lst == (4, 5, 6)
assert t == [1, 2, 3]

# Test slicing assignment
missing += 1
# lst = [1, 2, 3, 4, 5]
# lst[1:3] = [6, 7]
# assert lst == [1, 6, 7, 4, 5]

# Test append in a loop
lst = []
for i in range(5):
    lst.append(i)
assert lst == [0, 1, 2, 3, 4]

# Test inserting in a loop
missing += 1
# lst = []
# for i in range(5):
#     lst.insert(0, i)
# assert lst == [4, 3, 2, 1, 0]

# Test list slicing with step
missing += 1
# lst = [1, 2, 3, 4, 5, 6, 7, 8, 9]
# assert lst[::3] == [1, 4, 7]

# Test list iteration
lst = [1, 2, 3]
result = []
for x in lst:
    result.append(x * 2)
assert result == [2, 4, 6]

# Test changing list during iteration (dangerous, but should work)
lst = [1, 2, 3]
for i in range(len(lst)):
    lst[i] = lst[i] * 2
assert lst == [2, 4, 6]

# Test list mutation with append in a loop
lst = []
for _ in range(5):
    lst.append(0)
assert lst == [0, 0, 0, 0, 0]

# Test using all, any with list
missing += 1
# lst = [1, 2, 3, 4]
# assert all(x > 0 for x in lst)
# assert any(x == 0 for x in lst)

# Test list multiplication with zero
assert lst * 0 == []

# Test mixed list types (not recommended, but valid in Python)
lst = [1, "string", 3.14]
assert lst == [1, "string", 3.14]

# Test deep copy of nested list (without copy method, to check reference)
missing += 1
# import copy
# nested_list = [[1, 2], [3, 4]]
# copied_list = copy.deepcopy(nested_list)
# assert copied_list == nested_list
# assert copied_list is not nested_list
# assert copied_list[0] is not nested_list[0]

print(f'missing: {missing}')