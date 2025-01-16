# Test simple function definition and call
def simple_function():
    return 42

assert simple_function() == 42

# Test function with arguments passed by position
def add(a, b):
    return a + b

assert add(3, 5) == 8
assert add(-1, 1) == 0

# Test function with arguments passed by keyword
assert add(a=10, b=20) == 30
assert add(b=7, a=3) == 10

# Test function with default arguments
def power(base, exp=2):
    result = 1
    while exp > 0:
        result *= base
        exp -= 1
    return result

assert power(3) == 9
assert power(3, 3) == 27

# Test function with variable-length positional arguments (*args)
def multiply_all(*args):
    result = 1
    for i in args:
        result *= i
    return result

assert multiply_all(1, 2, 3) == 6
assert multiply_all() == 1

# Test function with variable-length keyword arguments (**kwargs)
def keyword_sum(**kwargs):
    result = 0
    for key in kwargs:
        result += kwargs[key]
    return result

assert keyword_sum(a=1, b=2, c=3) == 6
assert keyword_sum() == 0

# Test function with both *args and **kwargs
def combined_function(a, *args, **kwargs):
    args_total = 0
    for i in args:
        args_total += i
    kwargs_total = 0
    for key in kwargs:
        kwargs_total += kwargs[key]
    return a + args_total + kwargs_total

assert combined_function(1, 2, 3, x=4, y=5) == 15

# Test recursion (direct)
def factorial(n):
    if n == 0:
        return 1
    return n * factorial(n - 1)

assert factorial(5) == 120
assert factorial(0) == 1

# Test recursion (indirect)
def is_even(n):
    if n == 0:
        return True
    return is_odd(n - 1)

def is_odd(n):
    if n == 0:
        return False
    return is_even(n - 1)

assert is_even(10) is True
assert is_odd(11) is True
assert is_even(5) is False

# Test mutable default arguments
def append_to_list(value, lst=[]):
    lst.append(value)
    return lst

list1 = append_to_list(1)
list2 = append_to_list(2)
assert list1 == [1, 2]
assert list1 is list2

# Test immutable default arguments
def safe_append_to_list(value, lst=None):
    if lst is None:
        lst = []
    lst.append(value)
    return lst

list3 = safe_append_to_list(1)
list4 = safe_append_to_list(2)
assert list3 == [1]
assert list4 == [2]

# Test closures
def make_multiplier(factor):
    def multiply_by_factor(x):
        return x * factor
    return multiply_by_factor

times_two = make_multiplier(2)
assert times_two(3) == 6
assert times_two(0) == 0

# Test edge cases with argument handling
def edge_case_function(a, b=10, *args, **kwargs):
    return a, b, args, kwargs

result1 = edge_case_function(1)
assert result1[0] == 1
assert result1[1] == 10
assert result1[2] == ()
assert result1[3] == {}

result2 = edge_case_function(1, 2, 3, 4, x=5)
assert result2[0] == 1
assert result2[1] == 2
assert result2[2] == (3, 4)
assert result2[3] == {"x": 5}

# Test nested functions
def outer_function(a):
    def inner_function(b):
        return a + b
    return inner_function

nested = outer_function(10)
assert nested(5) == 15
assert nested(0) == 10

# Test default argument expressions (evaluated only once)
def default_arg_expression(a, lst=[]):
    lst.append(a)
    return lst

assert default_arg_expression(1) == [1]
assert default_arg_expression(2) == [1, 2]

# Test using default None value to create a new list
def resettable_append(value, lst=None):
    if lst is None:
        lst = []
    lst.append(value)
    return lst

assert resettable_append(1) == [1]
assert resettable_append(2) == [2]

# Test call with positional and keyword arguments mixed
def mixed_args(a, b, c):
    return a + b + c

assert mixed_args(1, b=2, c=3) == 6
assert mixed_args(1, 2, c=3) == 6
