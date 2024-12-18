/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -t -N in_array_set -H array_hash gperf/array_methods.txt  */
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

#line 1 "gperf/array_methods.txt"

#include <string.h>
#include "nova_array.h"
#line 6 "gperf/array_methods.txt"
struct ArrayMethod {
	const char *name;
	Value (*method)(int, Value*);	
};

#define TOTAL_KEYWORDS 16
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 7
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 39
/* maximum key range = 36, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
array_hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40,  0, 40, 40,
       0, 10, 40,  5, 40, 40, 40, 40,  0,  0,
      10, 20,  0, 20, 15,  0,  0, 25, 10, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40
    };
  return len + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[1]];
}

struct ArrayMethod *
in_array_set (register const char *str, register size_t len)
{
  static struct ArrayMethod wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 21 "gperf/array_methods.txt"
      {"_lt_", novaArrayLess},
#line 23 "gperf/array_methods.txt"
      {"_add_", novaArrayAdd},
      {""}, {""}, {""},
#line 19 "gperf/array_methods.txt"
      {"_gt_", novaArrayGreater},
      {""}, {""}, {""}, {""},
#line 22 "gperf/array_methods.txt"
      {"_le_", novaArrayLessEqual},
#line 27 "gperf/array_methods.txt"
      {"_len_", novaArrayLen},
#line 14 "gperf/array_methods.txt"
      {"insert", novaArrayInsert},
#line 26 "gperf/array_methods.txt"
      {"_setat_", novaArraySetAt},
      {""},
#line 20 "gperf/array_methods.txt"
      {"_ge_", novaArrayGreaterEqual},
      {""}, {""},
#line 25 "gperf/array_methods.txt"
      {"_getat_", novaArrayGetAt},
#line 13 "gperf/array_methods.txt"
      {"pop", novaArrayPop},
#line 18 "gperf/array_methods.txt"
      {"_ne_", novaArrayNotEqual},
      {""}, {""},
#line 15 "gperf/array_methods.txt"
      {"reverse", novaArrayReverse},
      {""},
#line 12 "gperf/array_methods.txt"
      {"push", novaArrayPush},
#line 24 "gperf/array_methods.txt"
      {"_mul_", novaArrayMultiply},
      {""}, {""}, {""},
#line 17 "gperf/array_methods.txt"
      {"_eq_", novaArrayEqual},
      {""}, {""}, {""}, {""},
#line 16 "gperf/array_methods.txt"
      {"sort", novaArraySort}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = array_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 28 "gperf/array_methods.txt"

