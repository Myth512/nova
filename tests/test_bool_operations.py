assert not True == False

assert not False == True

assert True == 1

assert False == 0

assert True + True == 2

assert True - False == 1

assert True * 10 == 10

assert False * 10 == 0

assert True / 1 == 1.0

assert not False / 1

assert str(True) == "True"

assert repr(True) == "True"

assert str(False) == "False"

assert repr(False) == "False"

assert bool(30) == True

assert bool(-30) == True

assert bool(0) == False

assert bool(0.0) == False

assert bool(0.1) == True

assert bool("some string") == True

assert bool("") == False

assert bool(" ") == True

assert bool([]) == False

assert bool([1, 2, 3]) == True

assert bool({}) == False

assert bool({"key": "value"}) == True

assert bool(()) == False

assert bool((1, 2)) == True

assert bool(None) == False

assert bool(NotImplemented) == True

assert type(True) is bool

assert type(False) is bool

assert isinstance(True, int)

assert isinstance(False, int)

assert True & True == True

assert True & False == False

assert False & False == False

assert True | False == True

assert False | False == False

assert True ^ True == False

assert True ^ False == True

assert not (True and False)

assert True or False

assert not (False or False)

assert (True and True) or False

assert not (False and True)
