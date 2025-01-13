a = 10
assert a == 10

a = 5
assert a == 5

a += 15
assert a == 20

a -= 5
assert a == 15

a *= 2
assert a == 30

a /= 3
assert a == 10.0

a //= 2
assert a == 5

a %= 3
assert a == 2

a **= 3
assert a == 8

a = 8
a &= 5
assert a == 0

a = 8
a |= 3
assert a == 11

a = 8
a ^= 3
assert a == 11

a = 8
a <<= 2
assert a == 32

a >>= 3
assert a == 4