# Simple if statement
a = 5
if a == 5:
    result = "Equal"
assert result == "Equal"

# If-else statement
a = 3
if a == 5:
    result = "Equal"
else:
    result = "Not Equal"
assert result == "Not Equal"

# If-elif-else statement
a = 10
if a == 5:
    result = "Equal"
elif a == 10:
    result = "Ten"
else:
    result = "Other"
assert result == "Ten"

# Nested if statement
a = 5
b = 10
if a == 5:
    if b == 10:
        result = "Nested Match"
assert result == "Nested Match"

# Combining logical operators (and, or)
a = 5
b = 10
if a == 5 and b == 10:
    result = "Both conditions are true"
assert result == "Both conditions are true"

# Using 'or' in if statement
a = 3
b = 10
if a == 3 or b == 5:
    result = "At least one condition is true"
assert result == "At least one condition is true"

# Checking truthy and falsy values
a = 0
if a:
    result = "Truthy"
else:
    result = "Falsy"
assert result == "Falsy"

# Using multiple conditions in elif
a = 5
b = 10
if a == 5 and b == 10:
    result = "Matched first condition"
elif a == 3 or b == 5:
    result = "Matched second condition"
else:
    result = "No match"
assert result == "Matched first condition"

# Nested if-elif-else with more conditions
a = 5
b = 10
c = 15
if a == 5:
    if b == 10:
        if c == 15:
            result = "Triple match"
        else:
            result = "b matched but c did not"
    else:
        result = "a matched but b did not"
else:
    result = "No match"
assert result == "Triple match"

# Combining logical operators (and, or, not)
a = 5
b = 10
if not a == 3 and (b == 10 or a == 5):
    result = "Complex condition passed"
assert result == "Complex condition passed"

# Multiple conditions with mixed types
a = 10
b = "hello"
if isinstance(a, int) and isinstance(b, str):
    result = "Type match"
assert result == "Type match"

# Conditional on multiple types (list, int, str)
a = [1, 2, 3]
b = 5
if isinstance(a, list) and b == 5:
    result = "List and int matched"
assert result == "List and int matched"