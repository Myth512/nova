missing = 0

# Test: Basic try-except with no exception
try:
    x = 10
except:
    assert False, "Exception block should not execute"
else:
    assert x == 10

# Test: Single exception raised and caught
try:
    raise ValueError("Error occurred")
except ValueError as e:
    assert str(e) == "Error occurred"
else:
    assert False, "Else block should not execute when exception is raised"

# Test: Exception not caught (no matching except block)
try:
    raise KeyError("Missing key")
except ValueError:
    assert False, "This block should not execute for a KeyError"
except KeyError:
    pass
else:
    assert False, "Else block should not execute when exception is raised"

# Test: Multiple except blocks
try:
    raise TypeError("Wrong type")
except ValueError:
    assert False, "This block should not execute for a TypeError"
except TypeError as e:
    assert str(e) == "Wrong type"

# Test: Multiple except blocks with no exception
try:
    y = 20
except ValueError:
    assert False, "This block should not execute"
except TypeError:
    assert False, "This block should not execute"
else:
    assert y == 20

# Test: try block with finally (no exception)
try:
    z = 30
finally:
    cleanup_done = True
assert z == 30
assert cleanup_done

# Test: try block with exception and finally
try:
    raise IndexError("Out of bounds")
except IndexError as e:
    assert str(e) == "Out of bounds"
finally:
    cleanup_done = True
assert cleanup_done

# Test: Exception in try, else block not executed
try:
    raise ValueError("Invalid value")
except ValueError:
    exception_handled = True
else:
    assert False, "Else block should not execute when exception is raised"
finally:
    finally_executed = True
assert exception_handled
assert finally_executed

# Test: No exception, both else and finally executed
try:
    w = 42
except:
    assert False, "Exception block should not execute"
else:
    success = True
finally:
    cleanup_done = True
assert w == 42
assert success
assert cleanup_done

# Test: Nested try-except blocks
try:
    try:
        raise KeyError("Nested exception")
    except KeyError as e:
        raise ValueError("Outer exception")
except ValueError as e:
    assert str(e) == "Outer exception"

# Test: Exception re-raised in except block
try:
    try:
        raise TypeError("Inner exception")
    except TypeError as e:
        assert str(e) == "Inner exception"
        raise
except TypeError as e:
    assert str(e) == "Inner exception"

# Test: Unhandled exception propagates
try:
    1 / 0
except ValueError:
    assert False, "This block should not execute for ZeroDivisionError"
except TypeError:
    assert False, "This block should not execute for ZeroDivisionError"
except ZeroDivisionError:
    pass

# Test: Catching generic Exception
try:
    raise ValueError("Generic exception")
except Exception as e:
    assert str(e) == "Generic exception"

# Test: finally block with exception in except block
try:
    raise KeyError("Outer exception")
except KeyError:
    exception_handled = True
finally:
    cleanup_done = True
assert cleanup_done

# Test: finally block with return statement
missing += 1
# def test_finally_return():
#     try:
#         return 1
#     finally:
#         return 2
# assert test_finally_return() == 2

# Test: finally block with break in loop
for i in range(5):
    try:
        if i == 2:
            break
    finally:
        loop_cleanup_done = True
assert loop_cleanup_done

# Test: Exception with else and nonlocal variable
def test_try_except_nonlocal():
    nonlocal_flag = False

    def inner():
        nonlocal nonlocal_flag
        try:
            pass
        except:
            assert False, "Exception block should not execute"
        else:
            nonlocal_flag = True

    inner()
    return nonlocal_flag

assert test_try_except_nonlocal() is True

# Test: Raising custom exceptions
missing += 1
# class CustomException(Exception):
#     pass

# try:
#     raise CustomException("Custom error")
# except CustomException as e:
#     assert str(e) == "Custom error"

# Test: try block with else, finally, and custom exception
try:
    result = 0
    try:
        result = 1
    except:
        assert False, "Exception block should not execute"
    else:
        result = 2
    finally:
        result = 3
    assert result == 3
except:
    assert False, "No exceptions should propagate here"

# Test: finally block with complex control flow
try:
    for i in range(5):
        if i == 3:
            break
finally:
    final_cleanup = True
assert final_cleanup
 
print(f'missing: {missing}')