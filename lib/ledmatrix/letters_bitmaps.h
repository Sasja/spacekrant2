#ifndef LETTERS_BITMAPS_H
#define LETTERS_BITMAPS_H

#include "letters.h"

/*
 * Font definition.
 * Conventions:
 * - Every character is max. 1 byte "wide" and always 6 bytes "high" (6 rows)
 *   (most characters are 4 bits wide, some are 3 bits wide, and some 5.)
 * - The LSB of the first byte of each character denotes whether the character
 *   is always followed by an empty column (value = 1; typically used for digits), 
 *   or whether "variable spacing" mode is active (value = 0).
 * - The LSBs of the five other bytes of each character represent the "length"
 *   of the character in bits (e.g. 0,0,0,1,1 = 3)
 */

extern ltr_bitmap_t ltr_lowercase[26];
extern ltr_bitmap_t ltr_uppercase[26];
extern ltr_bitmap_t ltr_digits[10];

extern ltr_bitmap_t ltr_space;
extern ltr_bitmap_t ltr_space_small;
extern ltr_bitmap_t ltr_dot;
extern ltr_bitmap_t ltr_quote;
extern ltr_bitmap_t ltr_comma;
extern ltr_bitmap_t ltr_colon;
extern ltr_bitmap_t ltr_semicolon;
extern ltr_bitmap_t ltr_exclamation;
extern ltr_bitmap_t ltr_dash;
extern ltr_bitmap_t ltr_at;

#endif /* _LETTERS_BITMAPS_H */
