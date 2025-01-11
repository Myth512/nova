assert "hello" == 'hello'

assert "hello" != "world"

assert "abc" < "abcd"

assert "abcd" > "abc"

assert "a" <= "a"

assert "a" <= "b"

assert "b" >= "b"

assert "b" >= "a"

assert "hello" + " " + "world" == "hello world"

assert "abc" * 3 == "abcabcabc"

assert 3 * "hi " == "hi hi hi "

assert "e" in "hello"

assert "z" not in "hello"

assert "hello"[0] == "h"

assert "hello"[1] == "e"

assert "hello"[-1] == "o"

# assert "hello"[1:4] == "ell"

# assert "hello"[::2] == "hlo"

# assert "hello"[::-1] == "olleh"

assert "hello".upper() == "HELLO"

assert "HELLO".lower() == "hello"

assert "hello world".title() == "Hello World"

assert "hello".capitalize() == "Hello"

assert "HELLO".casefold() == "hello"

assert "hello".swapcase() == "HELLO"

assert "  hello  ".strip() == "hello"

assert "xxhelloxx".strip("x") == "hello"

assert "hello".startswith("he")

assert "hello".endswith("lo")

assert "hello".find("l") == 2

assert "hello".rfind("l") == 3

assert "hello".index("l") == 2

assert "hello".rindex("l") == 3

assert "hello".count("l") == 2

assert "hello world".split() == ["hello", "world"]

assert "hello,world".split(",") == ["hello", "world"]

assert "  ".join(["hello", "world"]) == "hello  world"

assert "hello world".replace("world", "Python") == "hello Python"

assert "hello".zfill(8) == "000hello"

assert "12345".isdecimal()

assert "12345".isdigit()

assert "12345".isnumeric()

assert "abc".isalpha()

assert "abc123".isalnum()

assert "hello world".isspace() == False

assert "    ".isspace()

assert "Hello".istitle()

assert "HELLO".isupper()

assert "hello".islower()

assert "hello".center(10) == "  hello   "

assert "hello".ljust(10) == "hello     "

assert "hello".rjust(10) == "     hello"

assert f"{2 + 2 * 2}" == "6"

name = "Alice"
age = 30

assert f'my name is {name}, my age is {age}' == "my name is Alice, my age is 30"

assert f"{'hello'.upper()}" == "HELLO"

assert "hello\nworld" == "hello\nworld"

assert r"hello\nworld" == "hello\\nworld"

s = "hello"

assert s.upper() == "HELLO"

assert s == "hello"
