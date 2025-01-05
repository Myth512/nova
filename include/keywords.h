/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf gperf/keywords.txt  */
/* Computed positions: -k'1,$' */

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

#line 1 "gperf/keywords.txt"

#include <string.h>
#include "token.h"
#line 11 "gperf/keywords.txt"
struct Keyword {
	const char *name;
    TokenType type;
};

#define TOTAL_KEYWORDS 35
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 53
/* maximum key range = 52, duplicates = 0 */

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
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      15, 54, 54, 54, 54, 54, 54, 54, 25, 54,
      54, 54, 54, 54, 20, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 10,  0, 25,
       0,  5,  0,  0,  5,  0, 54,  0, 30,  0,
      15,  5,  0, 54, 20, 20,  0, 54, 54, 10,
      54,  5, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

const struct Keyword *
in_keyword_set (register const char *str, register size_t len)
{
  static const struct Keyword wordlist[] =
    {
      {""}, {""},
#line 35 "gperf/keywords.txt"
      {"if", TOKEN_IF},
#line 25 "gperf/keywords.txt"
      {"def", TOKEN_DEF},
#line 33 "gperf/keywords.txt"
      {"from", TOKEN_FROM},
#line 22 "gperf/keywords.txt"
      {"break", TOKEN_BREAK},
#line 38 "gperf/keywords.txt"
      {"import", TOKEN_IMPORT},
      {""},
#line 48 "gperf/keywords.txt"
      {"try", TOKEN_TRY},
#line 27 "gperf/keywords.txt"
      {"elif", TOKEN_ELIF},
#line 51 "gperf/keywords.txt"
      {"yield", TOKEN_YIELD},
#line 29 "gperf/keywords.txt"
      {"except", TOKEN_EXCEPT},
#line 31 "gperf/keywords.txt"
      {"finally", TOKEN_FINALLY},
#line 17 "gperf/keywords.txt"
      {"and", TOKEN_AND},
#line 28 "gperf/keywords.txt"
      {"else", TOKEN_ELSE},
#line 21 "gperf/keywords.txt"
      {"await", TOKEN_AWAIT},
#line 19 "gperf/keywords.txt"
      {"assert", TOKEN_ASSERT},
#line 36 "gperf/keywords.txt"
      {"in", TOKEN_IN},
#line 42 "gperf/keywords.txt"
      {"not", TOKEN_NOT},
#line 50 "gperf/keywords.txt"
      {"with", TOKEN_WITH},
#line 49 "gperf/keywords.txt"
      {"while", TOKEN_WHILE},
      {""},
#line 37 "gperf/keywords.txt"
      {"is", TOKEN_IS},
#line 32 "gperf/keywords.txt"
      {"for", TOKEN_FOR},
#line 44 "gperf/keywords.txt"
      {"pass", TOKEN_PASS},
#line 30 "gperf/keywords.txt"
      {"False", TOKEN_FALSE},
      {""},
#line 43 "gperf/keywords.txt"
      {"or", TOKEN_OR},
      {""},
#line 47 "gperf/keywords.txt"
      {"True", TOKEN_TRUE,},
#line 45 "gperf/keywords.txt"
      {"raise", TOKEN_RAISE},
      {""},
#line 18 "gperf/keywords.txt"
      {"as", TOKEN_AS},
#line 26 "gperf/keywords.txt"
      {"del", TOKEN_DEL},
#line 40 "gperf/keywords.txt"
      {"None", TOKEN_NONE},
      {""},
#line 34 "gperf/keywords.txt"
      {"global", TOKEN_GLOBAL},
      {""},
#line 24 "gperf/keywords.txt"
      {"continue", TOKEN_CONTINUE},
      {""},
#line 20 "gperf/keywords.txt"
      {"async", TOKEN_ASYNC},
#line 46 "gperf/keywords.txt"
      {"return", TOKEN_RETURN},
      {""}, {""}, {""}, {""},
#line 39 "gperf/keywords.txt"
      {"lambda", TOKEN_LAMBDA},
      {""}, {""}, {""},
#line 23 "gperf/keywords.txt"
      {"class", TOKEN_CLASS},
      {""}, {""},
#line 41 "gperf/keywords.txt"
      {"nonlocal", TOKEN_NONLOCAL}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
