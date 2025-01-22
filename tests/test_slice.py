# Test the slice class
s = slice(1, 5, 2)

assert isinstance(s, slice)

assert type(s) is slice

# Test attributes of slice
assert s.start == 1, f"Expected start=1, but got {s.start}"
assert s.stop == 5, f"Expected stop=5, but got {s.stop}"
assert s.step == 2, f"Expected step=2, but got {s.step}"

# Test slice indices
seq_len = 10
start, stop, step = s.indices(seq_len)
assert (start, stop, step) == (1, 5, 2), f"Expected (1, 5, 2), but got {(start, stop, step)}"

# Test slice indices with negative indices
s_neg = slice(-7, -2, 1)
start, stop, step = s_neg.indices(seq_len)
assert (start, stop, step) == (3, 8, 1), f"Expected (3, 8, 1), but got {(start, stop, step)}"

# Test slice indices with larger range
s_out = slice(-15, 15, 3)
start, stop, step = s_out.indices(seq_len)
assert (start, stop, step) == (0, 10, 3), f"Expected (0, 10, 3), but got {(start, stop, step)}"

# Test slicing a list
lst = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
assert lst[s] == [1, 3], f"Expected [1, 3], but got {lst[s]}"
assert lst[s_neg] == [3, 4, 5, 6, 7], f"Expected [3, 4, 5, 6, 7], but got {lst[s_neg]}"

# Test slicing a tuple
tpl = (0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
assert tpl[s] == (1, 3), f"Expected (1, 3), but got {tpl[s]}"
assert tpl[s_neg] == (3, 4, 5, 6, 7), f"Expected (3, 4, 5, 6, 7), but got {tpl[s_neg]}"

# Test slicing a string
string = "abcdefghij"
assert string[s] == "bd", f"Expected 'bd', but got {string[s]}"
assert string[s_neg] == "defgh", f"Expected 'defgh', but got {string[s_neg]}"

# Test slicing with no step
s_no_step = slice(2, 8)
assert lst[s_no_step] == [2, 3, 4, 5, 6, 7], f"Expected [2, 3, 4, 5, 6, 7], but got {lst[s_no_step]}"
assert tpl[s_no_step] == (2, 3, 4, 5, 6, 7), f"Expected (2, 3, 4, 5, 6, 7), but got {tpl[s_no_step]}"
assert string[s_no_step] == "cdefgh", f"Expected 'cdefgh', but got {string[s_no_step]}"

# Test slicing with step of -1 (reverse)
s_reverse = slice(None, None, -1)
assert lst[s_reverse] == lst[::-1], f"Expected reversed list, but got {lst[s_reverse]}"
assert tpl[s_reverse] == tpl[::-1], f"Expected reversed tuple, but got {tpl[s_reverse]}"
assert string[s_reverse] == string[::-1], f"Expected reversed string, but got {string[s_reverse]}"

# Test slicing with negative start and stop
s_neg_indices = slice(-8, -3)
assert lst[s_neg_indices] == [2, 3, 4, 5, 6], f"Expected [2, 3, 4, 5, 6], but got {lst[s_neg_indices]}"
assert tpl[s_neg_indices] == (2, 3, 4, 5, 6), f"Expected (2, 3, 4, 5, 6), but got {tpl[s_neg_indices]}"
assert string[s_neg_indices] == "cdefg", f"Expected 'cdefg', but got {string[s_neg_indices]}"

# Test slicing with step > 1
s_step = slice(1, 8, 2)
assert lst[s_step] == [1, 3, 5, 7], f"Expected [1, 3, 5, 7], but got {lst[s_step]}"
assert tpl[s_step] == (1, 3, 5, 7), f"Expected (1, 3, 5, 7), but got {tpl[s_step]}"
assert string[s_step] == "bdfh", f"Expected 'bdfh', but got {string[s_step]}"

# Test slicing empty cases
assert lst[slice(5, 5)] == [], f"Expected [], but got {lst[slice(5, 5)]}"
assert tpl[slice(5, 5)] == (), f"Expected (), but got {tpl[slice(5, 5)]}"
assert string[slice(5, 5)] == "", f"Expected '', but got {string[slice(5, 5)]}"

# Test slicing with step=0 (invalid)
try:
    lst[slice(1, 5, 0)]
    assert False, "Expected ValueError for step=0, but no exception was raised"
except ValueError:
    pass  # Expected behavior

# Test custom slicing (slice object reuse)
custom_slice = slice(2, None, 2)
assert lst[custom_slice] == [2, 4, 6, 8], f"Expected [2, 4, 6, 8], but got {lst[custom_slice]}"
assert tpl[custom_slice] == (2, 4, 6, 8), f"Expected (2, 4, 6, 8), but got {tpl[custom_slice]}"
assert string[custom_slice] == "cegi", f"Expected 'cegi', but got {string[custom_slice]}"

# Initialize test data
lst = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
tpl = (0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
string = "abcdefghij"

### Test [:]
# Full slice - should return the entire sequence
assert lst[:] == lst, f"Expected full list {lst}, but got {lst[:]}"
assert tpl[:] == tpl, f"Expected full tuple {tpl}, but got {tpl[:]}"
assert string[:] == string, f"Expected full string '{string}', but got '{string[:]}'"

### Test [::]
# Full slice with default step - should return the entire sequence
assert lst[::] == lst, f"Expected full list {lst}, but got {lst[::]}"
assert tpl[::] == tpl, f"Expected full tuple {tpl}, but got {tpl[::]}"
assert string[::] == string, f"Expected full string '{string}', but got '{string[::]}'"

### Test [::-1]
# Reverse the sequence
assert lst[::-1] == lst[::-1], f"Expected reversed list {lst[::-1]}, but got {lst[::-1]}"
assert tpl[::-1] == tpl[::-1], f"Expected reversed tuple {tpl[::-1]}, but got {tpl[::-1]}"
assert string[::-1] == string[::-1], f"Expected reversed string '{string[::-1]}', but got '{string[::-1]}'"

### Test [1:5]
# Slice from index 1 to 5
assert lst[1:5] == [1, 2, 3, 4], f"Expected [1, 2, 3, 4], but got {lst[1:5]}"
assert tpl[1:5] == (1, 2, 3, 4), f"Expected (1, 2, 3, 4), but got {tpl[1:5]}"
assert string[1:5] == "bcde", f"Expected 'bcde', but got '{string[1:5]}'"

### Test [2:8:2]
# Slice from index 2 to 8 with step 2
assert lst[2:8:2] == [2, 4, 6], f"Expected [2, 4, 6], but got {lst[2:8:2]}"
assert tpl[2:8:2] == (2, 4, 6), f"Expected (2, 4, 6), but got {tpl[2:8:2]}"
assert string[2:8:2] == "ceg", f"Expected 'ceg', but got '{string[2:8:2]}'"

### Test [5:]
# Slice from index 5 to the end
assert lst[5:] == [5, 6, 7, 8, 9], f"Expected [5, 6, 7, 8, 9], but got {lst[5:]}"
assert tpl[5:] == (5, 6, 7, 8, 9), f"Expected (5, 6, 7, 8, 9), but got {tpl[5:]}"
assert string[5:] == "fghij", f"Expected 'fghij', but got '{string[5:]}'"

### Test [:5]
# Slice from the start to index 5
assert lst[:5] == [0, 1, 2, 3, 4], f"Expected [0, 1, 2, 3, 4], but got {lst[:5]}"
assert tpl[:5] == (0, 1, 2, 3, 4), f"Expected (0, 1, 2, 3, 4), but got {tpl[:5]}"
assert string[:5] == "abcde", f"Expected 'abcde', but got '{string[:5]}'"

### Test [::2]
# Slice with step 2 from start to end
assert lst[::2] == [0, 2, 4, 6, 8], f"Expected [0, 2, 4, 6, 8], but got {lst[::2]}"
assert tpl[::2] == (0, 2, 4, 6, 8), f"Expected (0, 2, 4, 6, 8), but got {tpl[::2]}"
assert string[::2] == "acegi", f"Expected 'acegi', but got '{string[::2]}'"

### Test [1::3]
# Slice from index 1 to end with step 3
assert lst[1::3] == [1, 4, 7], f"Expected [1, 4, 7], but got {lst[1::3]}"
assert tpl[1::3] == (1, 4, 7), f"Expected (1, 4, 7), but got {tpl[1::3]}"
assert string[1::3] == "beh", f"Expected 'bdfh', but got '{string[1::3]}'"

### Test Edge Cases
# Test empty slice
assert lst[10:] == [], f"Expected [], but got {lst[10:]}"
assert tpl[10:] == (), f"Expected (), but got {tpl[10:]}"
assert string[10:] == "", f"Expected '', but got '{string[10:]}'"

# Test negative step with no bounds
assert lst[::-1] == lst[::-1], f"Expected reversed list {lst[::-1]}, but got {lst[::-1]}"
assert tpl[::-1] == tpl[::-1], f"Expected reversed tuple {tpl[::-1]}, but got {tpl[::-1]}"
assert string[::-1] == string[::-1], f"Expected reversed string '{string[::-1]}', but got '{string[::-1]}'"

# Test slicing out of bounds
assert lst[100:200] == [], f"Expected [], but got {lst[100:200]}"
assert tpl[100:200] == (), f"Expected (), but got {tpl[100:200]}"
assert string[100:200] == "", f"Expected '', but got '{string[100:200]}'"

# Test slicing with negative indices
assert lst[-5:-1] == [5, 6, 7, 8], f"Expected [5, 6, 7, 8], but got {lst[-5:-1]}"
assert tpl[-5:-1] == (5, 6, 7, 8), f"Expected (5, 6, 7, 8), but got {tpl[-5:-1]}"
assert string[-5:-1] == "fghi", f"Expected 'fghi', but got '{string[-5:-1]}'"