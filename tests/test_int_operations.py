assert 1 == 1

assert 3 != 4

assert 2 > 1

assert 2 >= 1

assert 2 >= 2

assert 4 < 5

assert 4 <= 5

assert 4 <= 5

assert +5 == 5

assert -5 == -5

assert +0 == -0

assert 1 + 1 == 2

assert 1 - 1 == 0

assert 2 * 3 == 6

assert 2 * -5 == -10

assert -15 * 3 == -45

assert -4 * -5 == 20

assert 10 / 2 == 5

assert 10 / 4 == 2.5

assert 8 // 3 == 2

assert 10 % 3 == 1

assert 3 ** 4 == 81

assert 2 << 3 == 16

assert 16 >> 2 == 4

assert 5 | 11 == 15

assert 5 ^ 11 == 14

assert 6 & 15 == 6

assert ~1 == -2

assert not 5 == 0

assert not 0 == 1

assert int(55) == 55

assert int(True) == 1

assert int(False) == 0

assert int(3.14) == 3

assert int("12345") == 12345

assert int("    -0052") == -52

assert type(5) is int

assert str(55) == "55"

assert repr(-42) == "-42"
