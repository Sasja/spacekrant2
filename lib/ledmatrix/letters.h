#ifndef LETTERS_H
#define LETTERS_H

#include <stdint.h>

// =========================== Constants ========================

// ============================ Macros ==========================

// ========================= API Typedefs =======================

typedef const uint8_t ltr_bitmap_t[6];
#include "letters_bitmaps.h"

// ========================= Bitmap Data ========================

// ====================== API Global Variables ==================
// extern uint8_t tmpl_someApiVariable;

// ========================== API methods =======================
#if defined(__cplusplus)
extern "C" {
#endif

ltr_bitmap_t *ltr_lookupBitmap(char c);
inline uint8_t ltr_getCharLength(ltr_bitmap_t *bitmap);

#if defined(__cplusplus)
}
#endif

#endif /* LETTERS_H */
