/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf gperf/methods_none.txt  */
/* Computed positions: -k'3' */

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

#line 1 "gperf/methods_none.txt"

#include <string.h>
#include "py_none.h"
#include "value.h"
#line 7 "gperf/methods_none.txt"
struct StaticAttribute;

#define TOTAL_KEYWORDS 6
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 16
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 16
/* maximum key range = 11, duplicates = 0 */

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
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17,  5,  0,
      17,  5, 17,  0,  0, 17, 17, 17, 17, 17,
       0, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17
    };
  return len + asso_values[(unsigned char)str[2]];
}

const struct StaticAttribute *
in_none_set (register const char *str, register size_t len)
{
  static const struct StaticAttribute wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""},
#line 15 "gperf/methods_none.txt"
      {"__ne__", PyNone_NotEqual, true},
      {""},
#line 17 "gperf/methods_none.txt"
      {"__hash__", PyNone_Hash, true},
#line 19 "gperf/methods_none.txt"
      {"__class__", PyNone_Class, false},
      {""},
#line 14 "gperf/methods_none.txt"
      {"__eq__", PyNone_Equal, true},
      {""},
#line 18 "gperf/methods_none.txt"
      {"__bool__", PyNone_ToBool, true},
      {""}, {""},
#line 16 "gperf/methods_none.txt"
      {"__getattribute__", PyNone_GetAttribute, true}
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
#line 20 "gperf/methods_none.txt"

