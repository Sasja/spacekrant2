#ifndef BITMAPS_H
#define BITMAPS_H

#include <stdint.h>

typedef struct btm_bitmap {
    uint8_t widthbytes;
    uint8_t height;
    uint8_t buffer[];
} btm_bitmap_t;

// ========================= Bitmap Data ========================

// ====================== API Global Variables ==================
// extern uint8_t tmpl_someApiVariable;

// ========================== API methods =======================
#if defined(__cplusplus)
extern "C" {
#endif

extern btm_bitmap_t btm_whitespaceLogo;

#if defined(__cplusplus)
}
#endif

#endif /* BITMAPS_H */
