/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -t -N in_array_set gperf/arrayMethods.txt  */
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

#line 1 "gperf/arrayMethods.txt"

#include <string.h>
#include "novaArray.h"
#line 6 "gperf/arrayMethods.txt"
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
hash (register const char *str, register size_t len)
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
#line 21 "gperf/arrayMethods.txt"
      {"_lt_", arrayLess},
#line 23 "gperf/arrayMethods.txt"
      {"_add_", arrayAdd},
      {""}, {""}, {""},
#line 19 "gperf/arrayMethods.txt"
      {"_gt_", arrayGreater},
      {""}, {""}, {""}, {""},
#line 22 "gperf/arrayMethods.txt"
      {"_le_", arrayLessEqual},
#line 27 "gperf/arrayMethods.txt"
      {"_len_", arrayLen},
#line 14 "gperf/arrayMethods.txt"
      {"insert", arrayInsert},
#line 26 "gperf/arrayMethods.txt"
      {"_setat_", arraySetAt},
      {""},
#line 20 "gperf/arrayMethods.txt"
      {"_ge_", arrayGreaterEqual},
      {""}, {""},
#line 25 "gperf/arrayMethods.txt"
      {"_getat_", arrayGetAt},
#line 13 "gperf/arrayMethods.txt"
      {"pop", arrayPop},
#line 18 "gperf/arrayMethods.txt"
      {"_ne_", arrayNotEqual},
      {""}, {""},
#line 15 "gperf/arrayMethods.txt"
      {"reverse", arrayReverse},
      {""},
#line 12 "gperf/arrayMethods.txt"
      {"push", arrayPush},
#line 24 "gperf/arrayMethods.txt"
      {"_mul_", arrayMultiply},
      {""}, {""}, {""},
#line 17 "gperf/arrayMethods.txt"
      {"_eq_", arrayEqual},
      {""}, {""}, {""}, {""},
#line 16 "gperf/arrayMethods.txt"
      {"sort", arraySort}
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
#line 28 "gperf/arrayMethods.txt"

