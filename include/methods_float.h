/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf gperf/methods_float.txt  */
/* Computed positions: -k'3-5' */

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

#line 1 "gperf/methods_float.txt"

#include <string.h>
#include "py_float.h"
#include "value.h"
#line 7 "gperf/methods_float.txt"
struct StaticAttribute;

#define TOTAL_KEYWORDS 28
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 13
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 52
/* maximum key range = 47, duplicates = 0 */

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
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53,  0, 53,  3,  5, 15,
       0,  5,  0,  0, 30,  0, 53, 53, 20,  0,
      10,  0, 15, 30,  0,  5,  0, 25, 53, 15,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
      53, 53, 53, 53, 53, 53
    };
  return len + asso_values[(unsigned char)str[4]] + asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[2]];
}

const struct StaticAttribute *
in_float_set (register const char *str, register size_t len)
{
  static const struct StaticAttribute wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""},
#line 16 "gperf/methods_float.txt"
      {"__gt__", PyFloat_Greater, true},
#line 28 "gperf/methods_float.txt"
      {"__mod__", PyFloat_Modulo, true},
#line 29 "gperf/methods_float.txt"
      {"__rmod__", PyFloat_RightModulo, true},
      {""},
#line 20 "gperf/methods_float.txt"
      {"__add__", PyFloat_Add, true},
#line 17 "gperf/methods_float.txt"
      {"__ge__", PyFloat_GreaterEqual, true},
#line 25 "gperf/methods_float.txt"
      {"__rtruediv__", PyFloat_RightTrueDivide, true},
#line 38 "gperf/methods_float.txt"
      {"__bool__", PyFloat_ToBool, true},
      {""}, {""},
#line 36 "gperf/methods_float.txt"
      {"__getattr__", PyFloat_GetAttribute, true},
#line 39 "gperf/methods_float.txt"
      {"__int__", PyFloat_ToInt, true},
#line 35 "gperf/methods_float.txt"
      {"__init__", PyFloat_Init, true},
      {""},
#line 41 "gperf/methods_float.txt"
      {"is_integer", PyFloat_IsInteger, true},
#line 15 "gperf/methods_float.txt"
      {"__ne__", PyFloat_NotEqual, true},
#line 33 "gperf/methods_float.txt"
      {"__neg__", PyFloat_Negative, true},
#line 31 "gperf/methods_float.txt"
      {"__rpow__", PyFloat_RightPower, true},
      {""}, {""},
#line 18 "gperf/methods_float.txt"
      {"__lt__", PyFloat_Less, true},
#line 32 "gperf/methods_float.txt"
      {"__pos__", PyFloat_Positive, true},
      {""},
#line 40 "gperf/methods_float.txt"
      {"__float__", PyFloat_ToFloat, true},
      {""},
#line 19 "gperf/methods_float.txt"
      {"__le__", PyFloat_LessEqual, true},
#line 26 "gperf/methods_float.txt"
      {"__floordiv__", PyFloat_FloorDivide, true},
#line 27 "gperf/methods_float.txt"
      {"__rfloordiv__", PyFloat_RightFloorDivide, true},
      {""}, {""},
#line 24 "gperf/methods_float.txt"
      {"__truediv__", PyFloat_TrueDivide, true},
#line 30 "gperf/methods_float.txt"
      {"__pow__", PyFloat_Power, true},
#line 22 "gperf/methods_float.txt"
      {"__rsub__", PyFloat_RightSubtract, true},
      {""}, {""},
#line 14 "gperf/methods_float.txt"
      {"__eq__", PyFloat_Equal, true},
#line 21 "gperf/methods_float.txt"
      {"__sub__", PyFloat_Subtract, true},
      {""}, {""}, {""},
#line 37 "gperf/methods_float.txt"
      {"__hash__", PyFloat_Hash, true},
#line 34 "gperf/methods_float.txt"
      {"__class__", PyFloat_Class, false},
      {""}, {""}, {""}, {""},
#line 23 "gperf/methods_float.txt"
      {"__mul__", PyFloat_Multiply, true}
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
