import operations

assert operations.add("Hello", " World") == 'Hello World'

assert operations.sub(25, 13) == 12

assert operations.mul([1, 2], 3) == [1, 2, 1, 2, 1, 2]

assert operations.div(3.75, 1.25) == 3

assert operations.inc() == 1
assert operations.inc() == 2
assert operations.inc() == 3

assert operations.dec() == 2
assert operations.dec() == 1

assert operations.inc() == 2