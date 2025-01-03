/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf gperf/methods_int.txt  */
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

#line 1 "gperf/methods_int.txt"

#include <string.h>
#include "object_string.h"
#include "vm.h"
#include "value.h"
#include "value_int.h"

Value PyInt_Equal(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Equal(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("==", argv[0], argv[1]);
    return res;
}

Value PyInt_NotEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_NotEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("!=", argv[0], argv[1]);
    return res;
}

Value PyInt_Greater(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Greater(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(">", argv[0], argv[1]);
    return res;
}

Value PyInt_GreaterEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_GreaterEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(">=", argv[0], argv[1]);
    return res;
}

Value PyInt_Less(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Less(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("<", argv[0], argv[1]);
    return res;
}

Value PyInt_LessEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_LessEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("<=", argv[0], argv[1]);
    return res;
}

Value PyInt_Add(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Add(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("+", argv[0], argv[1]);
    return res;
}

Value PyInt_Subtract(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Subtract(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("-", argv[0], argv[1]);
    return res;
}

Value PyInt_Multiply(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Multiply(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("*", argv[0], argv[1]);
    return res;
}

Value PyInt_TrueDivide(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_TrueDivide(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("/", argv[0], argv[1]);
    return res;
}

Value PyInt_FloorDivide(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_FloorDivide(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("//", argv[0], argv[1]);
    return res;
}

Value PyInt_Modulo(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Modulo(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("%", argv[0], argv[1]);
    return res;
}

Value PyInt_Power(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Power(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("**", argv[0], argv[1]);
    return res;
}

Value PyInt_Positive(int argc, Value *argv) {
    if (argc != 0)
        reportArityError(0, 0, argc);
    Value res = Int_Positive(argv[0]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplementedUnary("+", argv[0]);
    return res;
}

Value PyInt_Negative(int argc, Value *argv) {
    if (argc != 0)
        reportArityError(0, 0, argc);
    Value res = Int_Negative(argv[0]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplementedUnary("-", argv[0]);
    return res;
}

Value PyInt_And(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_And(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("&", argv[0], argv[1]);
    return res;
}

Value PyInt_Xor(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Xor(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("^", argv[0], argv[1]);
    return res;
}

Value PyInt_Or(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_Or(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("|", argv[0], argv[1]);
    return res;
}

Value PyInt_Invert(int argc, Value *argv) {
    if (argc != 0)
        reportArityError(0, 0, argc);
    Value res = Int_Invert(argv[0]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplementedUnary("~", argv[0]);
    return res;
}

Value PyInt_LeftShift(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_LeftShift(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("<<", argv[0], argv[1]);
    return res;
}

Value PyInt_RightShift(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_RightShift(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(">>", argv[0], argv[1]);
    return res;
}

Value PyInt_GetAttr(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = Int_GetAttr(argv[0], AS_STRING(argv[1]));
    return res;
}

#line 206 "gperf/methods_int.txt"
struct GperfMethod;

#define TOTAL_KEYWORDS 22
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 61
/* maximum key range = 55, duplicates = 0 */

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
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 25, 62,  5, 10, 62,
       0, 10, 20,  5,  0,  0, 62, 62,  0,  0,
      10, 20, 15, 20,  5,  0,  0,  0,  0,  5,
       5, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62
    };
  return len + asso_values[(unsigned char)str[4]] + asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[2]];
}

const struct GperfMethod *
in_int_set (register const char *str, register size_t len)
{
  static const struct GperfMethod wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 221 "gperf/methods_int.txt"
      {"__mul__", PyInt_Multiply},
      {""}, {""},
#line 232 "gperf/methods_int.txt"
      {"__lshift__", PyInt_LeftShift},
      {""},
#line 219 "gperf/methods_int.txt"
      {"__add__", PyInt_Add},
      {""}, {""},
#line 233 "gperf/methods_int.txt"
      {"__rshift__", PyInt_RightShift},
#line 222 "gperf/methods_int.txt"
      {"__truediv__", PyInt_TrueDivide},
#line 220 "gperf/methods_int.txt"
      {"__sub__", PyInt_Subtract},
      {""}, {""},
#line 231 "gperf/methods_int.txt"
      {"__invert__", PyInt_Invert},
      {""},
#line 228 "gperf/methods_int.txt"
      {"__and__", PyInt_And},
      {""}, {""}, {""},
#line 234 "gperf/methods_int.txt"
      {"__getattr__", PyInt_GetAttr},
#line 224 "gperf/methods_int.txt"
      {"__mod__", PyInt_Modulo},
      {""}, {""}, {""},
#line 217 "gperf/methods_int.txt"
      {"__lt__", PyInt_Less},
#line 227 "gperf/methods_int.txt"
      {"__neg__", PyInt_Negative},
      {""}, {""}, {""},
#line 215 "gperf/methods_int.txt"
      {"__gt__", PyInt_Greater},
#line 229 "gperf/methods_int.txt"
      {"__xor__", PyInt_Xor},
      {""}, {""}, {""},
#line 218 "gperf/methods_int.txt"
      {"__le__", PyInt_LessEqual},
#line 226 "gperf/methods_int.txt"
      {"__pos__", PyInt_Positive},
      {""}, {""}, {""},
#line 216 "gperf/methods_int.txt"
      {"__ge__", PyInt_GreaterEqual},
#line 225 "gperf/methods_int.txt"
      {"__pow__", PyInt_Power},
      {""}, {""}, {""},
#line 214 "gperf/methods_int.txt"
      {"__ne__", PyInt_NotEqual},
#line 223 "gperf/methods_int.txt"
      {"__floordiv__", PyInt_FloorDivide},
      {""}, {""}, {""},
#line 230 "gperf/methods_int.txt"
      {"__or__", PyInt_Or},
      {""}, {""}, {""}, {""},
#line 213 "gperf/methods_int.txt"
      {"__eq__", PyInt_Equal}
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
#line 235 "gperf/methods_int.txt"

