#ifndef DISPLAYMETHODS_H
#define DISPLAYMETHODS_H

#include <stdint.h>
#include "ledmatrix.h"
#include "letters.h"

// =========================== Constants ========================

// ============================ Macros ==========================

// ========================= API Typedefs =======================

// ====================== API Global Variables ==================
extern const uint8_t dspm_oneMasks[9];

// ========================== API methods =======================
#if defined(__cplusplus)
extern "C" {
#endif

uint8_t dspm_writeChar2Display(char c, ledm_display_t *display,
        int8_t rowOffset, int8_t colOffset);
uint8_t dspm_writeString2Display(char str[], ledm_display_t *display,
        int8_t rowOffset, int8_t colOffset);


#if defined(__cplusplus)
}
#endif

#endif /* DISPLAYMETHODS_H */
