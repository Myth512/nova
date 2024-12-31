/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf gperf/methods_string.txt  */
/* Computed positions: -k'3-4' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "gperf/methods_string.txt"

#include <string.h>
#include "object_string.h"
#include "vm.h"
#include "value.h"
#include "object_string.h"

Value PyString_Equal(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = String_Equal(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("==", argv[0], argv[1]);
    return res;
}

Value PyString_NotEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = String_NotEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("!=", argv[0], argv[1]);
    return res;
}

Value PyString_Greater(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = String_Greater(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(">", argv[0], argv[1]);
    return res;
}

Value PyString_GreaterEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = String_GreaterEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(">=", argv[0], argv[1]);
    return res;
}

Value PyString_Less(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = String_Less(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("<", argv[0], argv[1]);
    return res;
}

Value PyString_LessEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = String_LessEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("<=", argv[0], argv[1]);
    return res;
}

Value PyString_Add(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = String_Add(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("+", argv[0], argv[1]);
    return res;
}

Value PyString_Multiply(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = String_Multiply(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("*", argv[0], argv[1]);
    return res;
}

Value PyString_GetAttr(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = String_GetAttr(argv[0], AS_STRING(argv[1]));
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplementedUnary("getattr", argv[0]);
    return res;
}

#line 91 "gperf/methods_string.txt"
struct GperfMethod;

#define TOTAL_KEYWORDS 9
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 11
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 31
/* maximum key range = 26, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32,  5, 32, 32,
       0, 10, 32,  5, 32, 32, 32, 32,  0,  0,
       4, 32, 32, 15, 32, 32,  0,  0, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32
    };
  return len + asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[2]];
}

const struct GperfMethod *
in_string_set (register const char *str, register size_t len)
{
  static const struct GperfMethod wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""},
#line 102 "gperf/methods_string.txt"
      {"__lt__", PyString_Less},
#line 105 "gperf/methods_string.txt"
      {"__mul__", PyString_Multiply},
      {""}, {""}, {""},
#line 100 "gperf/methods_string.txt"
      {"__gt__", PyString_Greater},
#line 104 "gperf/methods_string.txt"
      {"__add__", PyString_Add},
      {""}, {""}, {""},
#line 103 "gperf/methods_string.txt"
      {"__le__", PyString_LessEqual},
      {""}, {""}, {""},
#line 99 "gperf/methods_string.txt"
      {"__ne__", PyString_NotEqual},
#line 101 "gperf/methods_string.txt"
      {"__ge__", PyString_GreaterEqual},
      {""}, {""}, {""}, {""},
#line 106 "gperf/methods_string.txt"
      {"__getattr__", PyString_GetAttr},
      {""}, {""}, {""}, {""},
#line 98 "gperf/methods_string.txt"
      {"__eq__", PyString_Equal}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 107 "gperf/methods_string.txt"

