#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "TimerOne_C.h"

// =========================== Constants ========================
#define LEDM_ROWS 6
#define LEDM_COLS 24

// ============================ Macros ==========================
#define LEDM_COLBYTES (LEDM_COLS / 8)

// ========================= API Typedefs =======================
typedef uint8_t ledm_buffer_t[LEDM_ROWS][LEDM_COLBYTES];

typedef struct _ledm_display {
    ledm_buffer_t buffer;
    volatile bool frameFinishedFlag;
    volatile bool rowFinishedFlag;
} ledm_display_t;

// ====================== API Global Variables ==================
// extern uint8_t tmpl_someApiVariable;

// ========================== API methods =======================
#if defined(__cplusplus)
extern "C" {
#endif

void ledm_setup();
void ledm_setDisp(ledm_display_t *display);

#if defined(__cplusplus)
}
#endif

#endif /* LEDMATRIX_H */
