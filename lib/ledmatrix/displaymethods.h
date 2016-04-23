#ifndef DISPLAYMETHODS_H
#define DISPLAYMETHODS_H

#include <stdint.h>
#include "ledmatrix.h"
#include "letters.h"
#include "bitmaps.h"

// =========================== Constants ========================

// ============================ Macros ==========================

// ========================= API Typedefs =======================

// ====================== API Global Variables ==================
extern const uint8_t dspm_oneMasks[9];

// ========================== API methods =======================
#if defined(__cplusplus)
extern "C" {
#endif

void dspm_clear(ledm_display_t *display);
void dspm_fill(ledm_display_t *display);
uint8_t dspm_writeChar2Display(char c, ledm_display_t *display,
        int8_t rowOffset, int16_t colOffset);
uint8_t dspm_writeCharBitmap2Display(ltr_bitmap_t *bitmap, ledm_display_t *display,
        int8_t rowOffset, int16_t colOffset);
uint16_t dspm_writeString2Display(char str[], ledm_display_t *display,
        int8_t rowOffset, int16_t colOffset);
void dspm_showBitmap(btm_bitmap_t *bitmap, ledm_display_t *display,
        int8_t rowOffset, int8_t colOffset);

#if defined(__cplusplus)
}
#endif

#endif /* DISPLAYMETHODS_H */
