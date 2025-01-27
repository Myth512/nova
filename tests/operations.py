def add(a, b):
    return a + b

def sub(a, b):
    return a - b

def mul(a, b):
    return a * b

def div(a, b):
    return a / b

c = 0

def inc():
    global c
    c += 1
    return c

def dec():
    global c
    c -= 1
    return c