/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf gperf/methods_string.txt  */
/* Computed positions: -k'1,3-5' */

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
#include "py_string.h"
#include "value.h"
#line 7 "gperf/methods_string.txt"
struct StaticAttribute;

#define TOTAL_KEYWORDS 61
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 16
#define MIN_HASH_VALUE 10
#define MAX_HASH_VALUE 131
/* maximum key range = 122, duplicates = 0 */

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
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132,   5, 132,  15, 132,  30,
       40,  20,  35,  41,   0,   0,   0,   0,  10,   5,
       15,  35,   0,   5,   5,   0,  10,  55, 132,  50,
       30, 132,  10, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct StaticAttribute *
in_string_set (register const char *str, register size_t len)
{
  static const struct StaticAttribute wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 68 "gperf/methods_string.txt"
      {"strip", PyString_Strip, true},
      {""}, {""}, {""}, {""}, {""},
#line 48 "gperf/methods_string.txt"
      {"isprintable", PyString_IsPrintable, true},
      {""}, {""},
#line 52 "gperf/methods_string.txt"
      {"join", PyString_Join, true},
      {""},
#line 63 "gperf/methods_string.txt"
      {"rsplit", PyString_Rsplit, true},
#line 49 "gperf/methods_string.txt"
      {"isspace", PyString_IsSpace, true},
      {""},
#line 57 "gperf/methods_string.txt"
      {"partition", PyString_Partition, true},
#line 65 "gperf/methods_string.txt"
      {"split", PyString_Split, true},
#line 64 "gperf/methods_string.txt"
      {"rstrip", PyString_Rstrip, true},
#line 50 "gperf/methods_string.txt"
      {"istitle", PyString_IsTitle, true},
#line 27 "gperf/methods_string.txt"
      {"__hash__", PyString_Hash, true},
      {""},
#line 66 "gperf/methods_string.txt"
      {"splitlines", PyString_Splitlines, true},
#line 55 "gperf/methods_string.txt"
      {"lstrip", PyString_Lstrip, true},
#line 41 "gperf/methods_string.txt"
      {"isalpha", PyString_IsAlpha, true},
      {""}, {""},
#line 73 "gperf/methods_string.txt"
      {"zfill", PyString_Zfill, true},
#line 18 "gperf/methods_string.txt"
      {"__lt__", PyString_Less, true},
#line 58 "gperf/methods_string.txt"
      {"replace", PyString_Replace, true},
      {""}, {""},
#line 67 "gperf/methods_string.txt"
      {"startswith", PyString_Startswith, true},
#line 14 "gperf/methods_string.txt"
      {"__eq__", PyString_Equal, true},
      {""},
#line 24 "gperf/methods_string.txt"
      {"__iter__", PyString_Iter, true},
#line 56 "gperf/methods_string.txt"
      {"maketrans", PyString_Maketrans, true},
#line 62 "gperf/methods_string.txt"
      {"rpartition", PyString_Rpartition, true},
#line 19 "gperf/methods_string.txt"
      {"__le__", PyString_LessEqual, true},
#line 40 "gperf/methods_string.txt"
      {"isalnum", PyString_IsAlnum, true},
      {""},
#line 71 "gperf/methods_string.txt"
      {"translate", PyString_Translate, true},
#line 29 "gperf/methods_string.txt"
      {"capitalize", PyString_Capitalize, true},
#line 15 "gperf/methods_string.txt"
      {"__ne__", PyString_NotEqual, true},
#line 42 "gperf/methods_string.txt"
      {"isascii", PyString_IsAscii, true},
#line 69 "gperf/methods_string.txt"
      {"swapcase", PyString_Swapcase, true},
      {""},
#line 70 "gperf/methods_string.txt"
      {"title", PyString_Title, true},
      {""},
#line 28 "gperf/methods_string.txt"
      {"__len__", PyString_Len, true},
      {""}, {""},
#line 35 "gperf/methods_string.txt"
      {"expandtabs", PyString_Expandtabs, true},
      {""},
#line 51 "gperf/methods_string.txt"
      {"isupper", PyString_IsUpper, true},
      {""}, {""},
#line 59 "gperf/methods_string.txt"
      {"rfind", PyString_Rfind, true},
#line 37 "gperf/methods_string.txt"
      {"format", PyString_Format, true},
#line 16 "gperf/methods_string.txt"
      {"__gt__", PyString_Greater, true},
      {""},
#line 74 "gperf/methods_string.txt"
      {"__class__", PyString_Class, false},
#line 38 "gperf/methods_string.txt"
      {"format_map", PyString_FormatMap, true},
      {""},
#line 45 "gperf/methods_string.txt"
      {"isidentifier", PyString_IsIdentifier, true},
      {""}, {""},
#line 61 "gperf/methods_string.txt"
      {"rjust", PyString_Rjust, true},
      {""},
#line 17 "gperf/methods_string.txt"
      {"__ge__", PyString_GreaterEqual, true},
#line 22 "gperf/methods_string.txt"
      {"__rmul__", PyString_RightMultiply, true},
      {""},
#line 53 "gperf/methods_string.txt"
      {"ljust", PyString_Ljust, true},
#line 31 "gperf/methods_string.txt"
      {"center", PyString_Center, true},
#line 21 "gperf/methods_string.txt"
      {"__mul__", PyString_Multiply, true},
      {""},
#line 47 "gperf/methods_string.txt"
      {"isnumeric", PyString_IsNumeric, true},
#line 72 "gperf/methods_string.txt"
      {"upper", PyString_Upper, true},
#line 60 "gperf/methods_string.txt"
      {"rindex", PyString_Rindex, true},
#line 26 "gperf/methods_string.txt"
      {"__getitem__", PyString_GetItem, true},
#line 44 "gperf/methods_string.txt"
      {"isdigit", PyString_IsDigit, true},
      {""},
#line 54 "gperf/methods_string.txt"
      {"lower", PyString_Lower, true},
      {""},
#line 25 "gperf/methods_string.txt"
      {"__getattribute__", PyString_GetAttribute, true },
#line 30 "gperf/methods_string.txt"
      {"casefold", PyString_CaseFold, true},
#line 36 "gperf/methods_string.txt"
      {"find", PyString_Find, true},
#line 39 "gperf/methods_string.txt"
      {"index", PyString_Index, true},
      {""},
#line 23 "gperf/methods_string.txt"
      {"__contains__", PyString_Contains, false},
      {""},
#line 43 "gperf/methods_string.txt"
      {"isdecimal", PyString_IsDecimal, true},
      {""}, {""},
#line 46 "gperf/methods_string.txt"
      {"islower", PyString_IsLower, true},
      {""}, {""}, {""}, {""},
#line 20 "gperf/methods_string.txt"
      {"__add__", PyString_Add, true},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 32 "gperf/methods_string.txt"
      {"count", PyString_Count, true},
      {""}, {""},
#line 34 "gperf/methods_string.txt"
      {"endswith", PyString_Endswith, true},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 33 "gperf/methods_string.txt"
      {"encode", PyString_Encode, true}
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
#line 75 "gperf/methods_string.txt"

