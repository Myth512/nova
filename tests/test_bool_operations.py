assert not True == False

assert not False == True

assert True == 1

assert False == 0

assert True + True == 2

assert str(True) == "True"

assert repr(True) == "True"

assert str(False) == "False"

assert repr(False) == "False"

assert bool(30) == True

assert bool(0) == False

assert bool("some string") == True

assert bool("") == False
