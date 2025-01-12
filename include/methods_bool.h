/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf gperf/methods_bool.txt  */
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

#line 1 "gperf/methods_bool.txt"

#include <string.h>
#include "py_bool.h"
#include "value.h"
#line 7 "gperf/methods_bool.txt"
struct StaticAttribute;

#define TOTAL_KEYWORDS 30
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 13
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 55
/* maximum key range = 50, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
bool_hash (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56,  0, 56, 30,  5,  0,
       5, 10,  0,  5,  5,  0, 56, 56,  0,  8,
       3,  3, 30, 13,  5, 15,  0, 25,  0, 10,
       5, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56
    };
  return len + asso_values[(unsigned char)str[4]] + asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[2]];
}

const struct StaticAttribute *
in_bool_set (register const char *str, register size_t len)
{
  static const struct StaticAttribute wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""},
#line 19 "gperf/methods_bool.txt"
      {"__lt__", PyBool_Less, true},
      {""}, {""}, {""}, {""},
#line 17 "gperf/methods_bool.txt"
      {"__gt__", PyBool_Greater, true},
      {""},
#line 38 "gperf/methods_bool.txt"
      {"__invert__", PyBool_Invert, true},
#line 37 "gperf/methods_bool.txt"
      {"__or__", PyBool_Or, true},
#line 27 "gperf/methods_bool.txt"
      {"__floordiv__", PyBool_FloorDivide, true},
#line 20 "gperf/methods_bool.txt"
      {"__le__", PyBool_LessEqual, true},
      {""},
#line 28 "gperf/methods_bool.txt"
      {"__rfloordiv__", PyBool_RightFloorDivide, true},
#line 16 "gperf/methods_bool.txt"
      {"__ne__", PyBool_NotEqual, true},
#line 36 "gperf/methods_bool.txt"
      {"__xor__", PyBool_Xor, true},
#line 18 "gperf/methods_bool.txt"
      {"__ge__", PyBool_GreaterEqual, true},
#line 26 "gperf/methods_bool.txt"
      {"__rtruediv__", PyBool_RightTrueDivide, true},
#line 29 "gperf/methods_bool.txt"
      {"__mod__", PyBool_Modulo, true},
#line 30 "gperf/methods_bool.txt"
      {"__rmod__", PyBool_RightModulo, true},
#line 34 "gperf/methods_bool.txt"
      {"__neg__", PyBool_Negative, true},
#line 43 "gperf/methods_bool.txt"
      {"__getattr__", PyBool_GetAttribute, true},
      {""}, {""},
#line 15 "gperf/methods_bool.txt"
      {"__eq__", PyBool_Equal, true},
#line 39 "gperf/methods_bool.txt"
      {"__lshift__", PyBool_LeftShift, true},
#line 40 "gperf/methods_bool.txt"
      {"__rlshift__", PyBool_RightLeftShift, true},
      {""}, {""}, {""},
#line 41 "gperf/methods_bool.txt"
      {"__rshift__", PyBool_RightShift, true},
#line 42 "gperf/methods_bool.txt"
      {"__rrshift__", PyBool_RightRightShift, true},
      {""}, {""},
#line 44 "gperf/methods_bool.txt"
      {"__class__", PyBool_Class, false},
#line 24 "gperf/methods_bool.txt"
      {"__mul__", PyBool_Multiply, true},
#line 25 "gperf/methods_bool.txt"
      {"__truediv__", PyBool_TrueDivide, true},
      {""}, {""}, {""},
#line 35 "gperf/methods_bool.txt"
      {"__and__", PyBool_And, true},
#line 32 "gperf/methods_bool.txt"
      {"__rpow__", PyBool_RightPower, true},
#line 21 "gperf/methods_bool.txt"
      {"__add__", PyBool_Add, true},
      {""}, {""},
#line 31 "gperf/methods_bool.txt"
      {"__pow__", PyBool_Power, true},
      {""},
#line 22 "gperf/methods_bool.txt"
      {"__sub__", PyBool_Subtract, true},
#line 23 "gperf/methods_bool.txt"
      {"__rsub__", PyBool_RightSubtract, true},
      {""},
#line 33 "gperf/methods_bool.txt"
      {"__pos__", PyBool_Positive, true}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = bool_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
