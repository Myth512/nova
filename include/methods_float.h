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
#include "object_string.h"
#include "vm.h"
#include "value.h"
#include "value_float.h"

Value PyFloat_Equal(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_Equal(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("==", argv[0], argv[1]);
    return res;
}

Value PyFloat_NotEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_NotEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("!=", argv[0], argv[1]);
    return res;
}

Value PyFloat_Greater(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_Greater(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(">", argv[0], argv[1]);
    return res;
}

Value PyFloat_GreaterEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_GreaterEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(">=", argv[0], argv[1]);
    return res;
}

Value PyFloat_Less(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_Less(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("<", argv[0], argv[1]);
    return res;
}

Value PyFloat_LessEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_LessEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("<=", argv[0], argv[1]);
    return res;
}

Value PyFloat_Add(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_Add(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("+", argv[0], argv[1]);
    return res;
}

Value PyFloat_Subtract(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_Subtract(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("-", argv[0], argv[1]);
    return res;
}

Value PyFloat_Multiply(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_Multiply(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("*", argv[0], argv[1]);
    return res;
}

Value PyFloat_TrueDivide(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_TrueDivide(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("/", argv[0], argv[1]);
    return res;
}

Value PyFloat_FloorDivide(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_FloorDivide(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("//", argv[0], argv[1]);
    return res;
}

Value PyFloat_Modulo(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_Modulo(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("%", argv[0], argv[1]);
    return res;
}

Value PyFloat_Power(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_Power(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("**", argv[0], argv[1]);
    return res;
}

Value PyFloat_Positive(int argc, Value *argv) {
    if (argc != 0)
        reportArityError(0, 0, argc);
    Value res = Float_Positive(argv[0]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplementedUnary("+", argv[0]);
    return res;
}

Value PyFloat_Negative(int argc, Value *argv) {
    if (argc != 0)
        reportArityError(0, 0, argc);
    Value res = Float_Negative(argv[0]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplementedUnary("-", argv[0]);
    return res;
}

Value PyFloat_GetAttr(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Float_GetAttr(argv[0], AS_STRING(argv[1]));
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplementedUnary("getattr", argv[0]);
    return res;
}

#line 154 "gperf/methods_float.txt"
struct GperfMethod;

#define TOTAL_KEYWORDS 16
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 56
/* maximum key range = 50, duplicates = 0 */

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
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 25, 57, 15, 25, 57,
      10, 10, 10,  5, 57, 57, 57, 57,  0, 10,
      10,  0,  0, 15,  0,  5,  0,  0, 57,  0,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57
    };
  return len + asso_values[(unsigned char)str[4]] + asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[2]];
}

const struct GperfMethod *
in_float_set (register const char *str, register size_t len)
{
  static const struct GperfMethod wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 173 "gperf/methods_float.txt"
      {"__pow__", PyFloat_Power},
      {""}, {""}, {""},
#line 170 "gperf/methods_float.txt"
      {"__truediv__", PyFloat_TrueDivide},
#line 174 "gperf/methods_float.txt"
      {"__pos__", PyFloat_Positive},
      {""}, {""}, {""}, {""},
#line 169 "gperf/methods_float.txt"
      {"__mul__", PyFloat_Multiply},
      {""}, {""}, {""}, {""},
#line 171 "gperf/methods_float.txt"
      {"__floordiv__", PyFloat_FloorDivide},
      {""}, {""}, {""},
#line 176 "gperf/methods_float.txt"
      {"__getattr__", PyFloat_GetAttr},
#line 172 "gperf/methods_float.txt"
      {"__mod__", PyFloat_Modulo},
      {""}, {""}, {""},
#line 165 "gperf/methods_float.txt"
      {"__lt__", PyFloat_Less},
#line 175 "gperf/methods_float.txt"
      {"__neg__", PyFloat_Negative},
      {""}, {""}, {""},
#line 163 "gperf/methods_float.txt"
      {"__gt__", PyFloat_Greater},
#line 168 "gperf/methods_float.txt"
      {"__sub__", PyFloat_Subtract},
      {""}, {""}, {""},
#line 166 "gperf/methods_float.txt"
      {"__le__", PyFloat_LessEqual},
#line 167 "gperf/methods_float.txt"
      {"__add__", PyFloat_Add},
      {""}, {""}, {""},
#line 164 "gperf/methods_float.txt"
      {"__ge__", PyFloat_GreaterEqual},
      {""}, {""}, {""}, {""},
#line 162 "gperf/methods_float.txt"
      {"__ne__", PyFloat_NotEqual},
      {""}, {""}, {""}, {""},
#line 161 "gperf/methods_float.txt"
      {"__eq__", PyFloat_Equal}
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
#line 177 "gperf/methods_float.txt"

