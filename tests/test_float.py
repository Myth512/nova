assert 2.5 == 2.5

assert 2 == 2.0

assert 3.75 != 3.14

assert 2.75 > 1.3

assert 3 > 2.5

assert 4.5 >= 1.5

assert 5 > -0.5

assert 6.6 >= 6.6

assert 10 >= 9.9

assert 15.1 < 123.456

assert 10 < 10.1

assert 15.1 <= 123.456

assert 123.456 <= 123.456

assert 1.5 + 2.5 == 4.0

assert 3 + 0.5 == 3.5

assert 3.14 - 1.14 == 2.0

assert 1 - 0.5 == 0.5

assert 2.5 * 3.75 == 9.375

assert 3 * 1.5 == 4.5

assert 7.7 / 2.2 == 3.5

assert 6 / 1.5 == 4.0

assert 8.1 // 3.1 == 2

assert 10 // 3.9 == 2

assert 10.5 % 3 == 1.5

assert 10 % 1.5 == 1.0 

assert 3.21 % 1.23 == 0.75

assert 10.5 ** 2 == 110.25 

assert 3.0 ** 4.0 == 81

assert +3.0 == 3.0

assert -2 == -(2)

assert +0.0 == -0.0

assert float(3) == 3.0

assert float(True) == 1.0

assert float(False) == 0.0

assert float("123.456") == 123.456

assert float("      -52.99") == -52.99

assert str(123.456) == "123.456"

assert repr(-10.5) == "-10.5"

assert type(3.333) is float

assert 3.14159.is_integer() == False

assert (0.5 + 0.5).is_integer() == True

assert (0.5 - 0.5).is_integer() == True