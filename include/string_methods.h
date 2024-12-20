/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -t -N in_string_set -H string_hash gperf/string_methods.txt  */
/* Computed positions: -k'2-3' */

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

#line 1 "gperf/string_methods.txt"

#include <string.h>
#include "nova_string.h"
#line 6 "gperf/string_methods.txt"
struct StringMethod {
	const char *name;
	Value (*method)(int, Value*);	
};

#define TOTAL_KEYWORDS 13
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 7
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 29
/* maximum key range = 26, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
string_hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30,  0, 30, 30,
       5, 10, 30,  5,  0, 30, 30, 30,  0,  0,
      15, 30, 10, 10, 30, 30,  0,  0,  0, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
      30, 30, 30, 30, 30, 30
    };
  return len + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[1]];
}

struct StringMethod *
in_string_set (register const char *str, register size_t len)
{
  static struct StringMethod wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 18 "gperf/string_methods.txt"
      {"_lt_", novaStringLess},
#line 21 "gperf/string_methods.txt"
      {"_mul_", novaStringMultiply},
#line 24 "gperf/string_methods.txt"
      {"_hash_", novaStringHash},
      {""}, {""},
#line 16 "gperf/string_methods.txt"
      {"_gt_", novaStringGreater},
#line 20 "gperf/string_methods.txt"
      {"_add_", novaStringAdd},
      {""}, {""}, {""},
#line 19 "gperf/string_methods.txt"
      {"_le_", novaStringLessEqual},
#line 23 "gperf/string_methods.txt"
      {"_len_", novaStringLen},
      {""},
#line 12 "gperf/string_methods.txt"
      {"reverse", novaStringReverse},
      {""},
#line 17 "gperf/string_methods.txt"
      {"_ge_", novaStringGreaterEqual},
      {""}, {""},
#line 22 "gperf/string_methods.txt"
      {"_getat_", novaStringGetAt},
      {""},
#line 14 "gperf/string_methods.txt"
      {"_eq_", novaStringEqual},
      {""}, {""},
#line 13 "gperf/string_methods.txt"
      {"replace", novaStringReplace},
      {""},
#line 15 "gperf/string_methods.txt"
      {"_ne_", novaStringNotEqual}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = string_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 25 "gperf/string_methods.txt"

