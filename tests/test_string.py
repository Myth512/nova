missing = 0

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

assert "hello"[1:4] == "ell"

assert "hello"[::2] == "hlo"

assert "hello"[::-1] == "olleh"

assert "hello".upper() == "HELLO"

assert "HELLO".lower() == "hello"

missing += 1
# assert "hello world".title() == "Hello World"

assert "hello".capitalize() == "Hello"

missing += 1
# assert "HELLO".casefold() == "hello"

missing += 1
# assert "hello".swapcase() == "HELLO"

missing += 1
# assert "  hello  ".strip() == "hello"

missing += 1
# assert "xxhelloxx".strip("x") == "hello"

missing += 1
# assert "hello".startswith("he")

missing += 1
# assert "hello".endswith("lo")

missing += 1
# assert "hello".find("l") == 2

missing += 1
# assert "hello".rfind("l") == 3

missing += 1
# assert "hello".index("l") == 2

missing += 1
# assert "hello".rindex("l") == 3

missing += 1
# assert "hello".count("l") == 2

missing += 1
# assert "hello world".split() == ["hello", "world"]

missing += 1
# assert "hello,world".split(",") == ["hello", "world"]

missing += 1
# assert "  ".join(["hello", "world"]) == "hello  world"

missing += 1
# assert "hello world".replace("world", "Python") == "hello Python"

missing += 1
# assert "hello".zfill(8) == "000hello"

missing += 1
# assert "12345".isdecimal()

missing += 1
# assert "12345".isdigit()

missing += 1
# assert "12345".isnumeric()

missing += 1
# assert "abc".isalpha()

missing += 1
# assert "abc123".isalnum()

missing += 1
# assert "hello world".isspace() == False

missing += 1
# assert "    ".isspace()

missing += 1
# assert "Hello".istitle()

missing += 1
# assert "HELLO".isupper()

missing += 1
# assert "hello".islower()

missing += 1
# assert "hello".center(10) == "  hello   "

missing += 1
# assert "hello".ljust(10) == "hello     "

missing += 1
# assert "hello".rjust(10) == "     hello"

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

print(f'missing: {missing}')