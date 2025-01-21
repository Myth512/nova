# Basic arithmetic precedence
assert 3 + 2 * 5 == 13  # Multiplication before addition
assert (3 + 2) * 5 == 25  # Parentheses first
assert 2 ** 3 * 2 == 16  # Exponentiation before multiplication
assert 4 + 3 * 2 ** 2 == 16  # Exponentiation first, then multiplication, then addition

# Unary negation vs multiplication
assert -2 * 3 == -6  # Multiplication before negation
assert - (2 * 3) == -6  # Parentheses around multiplication first
assert -2 ** 2 == -4  # Exponentiation before negation (right-associative)
assert -(2 ** 2) == -4  # Parentheses around exponentiation first

# Addition and subtraction precedence
assert 3 + 2 - 5 == 0  # Addition and subtraction have the same precedence, left to right
assert 5 - 2 + 3 == 6  # Addition and subtraction are left to right

# Combining addition, subtraction, and multiplication
assert 2 + 3 * 4 - 5 == 9  # Multiplication first, then addition and subtraction
assert (2 + 3) * (4 - 5) == -5  # Parentheses first, then multiplication

# Mixing negation, multiplication, and addition
assert 5 + -3 * 2 == -1  # Multiplication before negation, then addition
assert -(3 + 2) * 2 == -10  # Parentheses first, then negation and multiplication

# Power operator precedence
assert 2 + 3 ** 2 == 11  # Exponentiation before addition
assert (2 + 3) ** 2 == 25  # Parentheses first, then exponentiation

# Complex mixed operations with addition, subtraction, multiplication, and negation
assert 2 + 3 * 4 - 2 ** 2 == 10  # Exponentiation before multiplication, then addition and subtraction
assert (2 + 3 * (4 - 2)) ** 2 == 64  # Parentheses first, then multiplication, then exponentiation

# Using function calls and operation precedence
# assert max(2 + 3, 1 * 2) == 5  # Parentheses for function call first, then addition and multiplication
# assert min(2 ** 2, 3 + 4) == 7  # Exponentiation first in the function argument

assert not 3 + 2 * 5 == 11

# Mixing indexing and operations
lst = [1, 2, 3, 4]
assert lst[2] + 3 == 6  # Indexing first, then addition
assert lst[2] * 3 + 2 == 11  # Indexing first, multiplication, then addition
assert lst[2] ** 2 + 1 == 10  # Indexing first, exponentiation, then addition

# Calling functions and combining with other operations
def square(x):
    return x * x

assert square(3 + 2) == 25  # Parentheses first, then call, then multiplication inside the function
assert (square(3) + square(2)) * 2 == 26  # Calls first, then addition, then multiplication

# Combining bitwise operations with arithmetic
assert 3 + 2 * 5 & 7 == 5  # Multiplication first, then addition and bitwise AND
assert (3 + 2) * 5 & 7 == 1  # Parentheses first, then multiplication and bitwise AND
assert 2 ** 3 | 1 == 9  # Exponentiation first, then bitwise OR

# Logical operations with arithmetic
assert 3 + 2 * 5 and 4 == 4  # Multiplication first, then addition, then logical AND
assert 3 + 2 * 5 or 0 == 13  # Multiplication first, then addition, then logical OR

# Order of operations with multiple parentheses and different operators
assert (3 + 2) * (5 - 3) ** 2 == 20  # Parentheses first, then exponentiation, then multiplication
assert (3 + (2 * 5)) - 3 ** 2 == 4  # Parentheses first, then multiplication, then exponentiation

# Using multiple operations in the same line with complex precedence
assert 2 ** 3 * (3 + 4) - 5 == 51  # Exponentiation first, then multiplication, then addition and subtraction
assert (2 + 3) * 4 - 2 ** 2 == 16  # Parentheses first, then multiplication, then exponentiation

# Tricky case with nested function calls and precedence
def add(a, b):
    return a + b

assert add(3 * 2, 5 + 1) == 12  # Multiplication and addition are handled before the function call
assert add(2 ** 3, 4) == 12  # Exponentiation first, then function call

assert -1 - -2 == 1

# Tricky case with assignment and operations
a = 5
b = a + 2 * 3  # Multiplication first, then addition
assert b == 11
a += 2 ** 3  # Exponentiation first, then addition
assert a == 13
