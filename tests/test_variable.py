missing = 0

# Test global scope
global_var = 10

def test_global():
    global global_var
    global_var = 20

test_global()
assert global_var == 20

# Test local scope
def test_local():
    local_var = 10
    assert local_var == 10

test_local()

# Test accessing global variable from local scope
def access_global():
    assert global_var == 20

access_global()

# Test modifying global variable without 'global' keyword (should create a local variable instead)
def modify_global_without_keyword():
    global_var = 30  # Local variable, shadows global_var
    assert global_var == 30

modify_global_without_keyword()
assert global_var == 20  # Ensure global_var is unchanged

# Test modifying global variable with 'global' keyword
def modify_global_with_keyword():
    global global_var
    global_var = 40

modify_global_with_keyword()
assert global_var == 40

# Test nested functions with local scope
def outer_function():
    outer_var = 10

    def inner_function():
        inner_var = 20
        assert inner_var == 20
        assert outer_var == 10

    inner_function()

outer_function()

# Test nonlocal keyword in nested functions
def test_nonlocal():
    outer_var = 5

    def inner_function():
        nonlocal outer_var
        outer_var = 15

    inner_function()
    assert outer_var == 15

test_nonlocal()

# Test nonlocal keyword with multiple levels of nesting
def test_nonlocal_multiple_levels():
    level_1_var = 50

    def level_2_function():
        level_2_var = 100

        def level_3_function():
            nonlocal level_1_var
            level_1_var = 200

        level_3_function()
        assert level_1_var == 200
        assert level_2_var == 100  # Ensure nonlocal doesn't affect unrelated variables

    level_2_function()
    assert level_1_var == 200

test_nonlocal_multiple_levels()

# Test closures with access to outer variables
def create_closure():
    outer_var = 10

    def closure():
        return outer_var

    return closure

closure_func = create_closure()
assert closure_func() == 10

# Test closures with modifications to outer variables (nonlocal)
def create_modifying_closure():
    outer_var = 5

    def closure():
        nonlocal outer_var
        outer_var += 5
        return outer_var

    return closure

modifying_closure = create_modifying_closure()
assert modifying_closure() == 10
assert modifying_closure() == 15

# Test accessing global variables from a closure
global_var_closure = 50

def create_global_access_closure():
    def closure():
        return global_var_closure
    return closure

global_access_closure = create_global_access_closure()
assert global_access_closure() == 50

# Test modifying global variables from a closure
def create_global_modifying_closure():
    def closure():
        global global_var_closure
        global_var_closure += 10
    return closure

global_modifying_closure = create_global_modifying_closure()
global_modifying_closure()
assert global_var_closure == 60

# Test shadowing in nested functions
def test_shadowing():
    x = 10

    def inner_function():
        x = 20  # Shadows outer x
        assert x == 20

    inner_function()
    assert x == 10  # Ensure outer x is unchanged

test_shadowing()

# Test interaction between global and nonlocal
def test_global_nonlocal_interaction():
    global y
    y = 5

    def outer_function():
        y = 10  # Local to outer_function

        def inner_function():
            nonlocal y  # Refers to y in outer_function
            y += 5

        inner_function()
        assert y == 15

    outer_function()
    assert y == 5  # Ensure global y is unchanged

test_global_nonlocal_interaction()

# Test multiple closures sharing the same outer scope
global_increment = None
global_decrement = None
def create_multiple_closures():
    counter = 0

    def increment():
        nonlocal counter
        counter += 1
        return counter

    def decrement():
        nonlocal counter
        counter -= 1
        return counter

    global global_increment
    global global_decrement

    global_increment = increment
    global_decrement = decrement


create_multiple_closures()

assert global_increment() == 1
assert global_increment() == 2
assert global_decrement() == 1
assert global_decrement() == 0

# Test closures capturing values at definition time
missing += 1
# def create_closures_with_captured_values():
#     closures = []
#     for i in range(3):
#         def closure():
#             return i
#         closures.append(closure)
#     return closures

# closures = create_closures_with_captured_values()
# assert closures[0]() == 2  # i is captured by reference, not value
# assert closures[1]() == 2
# assert closures[2]() == 2

# Fix for capturing values at definition time
missing += 1
# def create_fixed_closures_with_captured_values():
#     closures = []
#     for i in range(3):
#         def closure(x=i):  # Capture value explicitly
#             return x
#         closures.append(closure)
#     return closures

# fixed_closures = create_fixed_closures_with_captured_values()
# assert fixed_closures[0]() == 0
# assert fixed_closures[1]() == 1
# assert fixed_closures[2]() == 2

print(f'missing: {missing}')