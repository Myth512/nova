/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf gperf/methods_list.txt  */
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

#line 1 "gperf/methods_list.txt"

#include <string.h>
#include "object_string.h"
#include "vm.h"
#include "value.h"
#include "object_list.h"

Value PyList_Equal(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = List_Equal(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("==", argv[0], argv[1]);
    return res;
}

Value PyList_NotEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = List_NotEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("!=", argv[0], argv[1]);
    return res;
}

Value PyList_Greater(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = List_Greater(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(">", argv[0], argv[1]);
    return res;
}

Value PyList_GreaterEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = List_GreaterEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented(">=", argv[0], argv[1]);
    return res;
}

Value PyList_Less(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = List_Less(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("<", argv[0], argv[1]);
    return res;
}

Value PyList_LessEqual(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = List_LessEqual(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("<=", argv[0], argv[1]);
    return res;
}

Value PyList_Add(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = List_Add(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("+", argv[0], argv[1]);
    return res;
}

Value PyList_Multiply(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = List_Multiply(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("*", argv[0], argv[1]);
    return res;
}

Value PyList_Append(int argc, Value *argv) {
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = List_Append(argv[0], argv[1]);
    return res;
}

Value PyList_Pop(int argc, Value *argv) {
    if (argc != 0)
        reportArityError(0, 0, argc);
    Value res = List_Pop(argv[0]);
    return res;
}

Value PyList_Sort(int argc, Value *argv) {
    if (argc != 0)
        reportArityError(0, 0, argc);
    Value res = List_Sort(argv[0]);
    return res;
}

#line 103 "gperf/methods_list.txt"
struct GperfMethod;

#define TOTAL_KEYWORDS 11
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 7
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 26
/* maximum key range = 24, duplicates = 0 */

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
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27,  5, 27, 27,
       0,  0, 27, 15, 27, 27, 27, 27,  5,  0,
       9, 27,  0,  4,  0, 27,  5,  0, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
      27, 27, 27, 27, 27, 27
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
        break;
    }
  return hval;
}

const struct GperfMethod *
in_list_set (register const char *str, register size_t len)
{
  static const struct GperfMethod wordlist[] =
    {
      {""}, {""}, {""},
#line 119 "gperf/methods_list.txt"
      {"pop", PyList_Pop},
      {""}, {""},
#line 118 "gperf/methods_list.txt"
      {"append", PyList_Append},
#line 117 "gperf/methods_list.txt"
      {"__mul__", PyList_Multiply},
      {""},
#line 120 "gperf/methods_list.txt"
      {"sort", PyList_Sort},
#line 110 "gperf/methods_list.txt"
      {"__eq__", PyList_Equal},
#line 115 "gperf/methods_list.txt"
      {"__le__", PyList_LessEqual},
#line 116 "gperf/methods_list.txt"
      {"__add__", PyList_Add},
      {""}, {""},
#line 111 "gperf/methods_list.txt"
      {"__ne__", PyList_NotEqual},
#line 114 "gperf/methods_list.txt"
      {"__lt__", PyList_Less},
      {""}, {""}, {""}, {""},
#line 113 "gperf/methods_list.txt"
      {"__ge__", PyList_GreaterEqual},
      {""}, {""}, {""}, {""},
#line 112 "gperf/methods_list.txt"
      {"__gt__", PyList_Greater}
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
#line 121 "gperf/methods_list.txt"

