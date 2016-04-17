#ifndef SPACEROUTINES_H
#define SPACEROUTINES_H

#include <stdint.h>
#include "ledmatrix.h"

// =========================== Constants ========================

// ============================ Macros ==========================

// ========================= API Typedefs =======================
typedef struct spr_routine {
    void (*init)(ledm_display_t *dsp);
    void (*handleData)(uint8_t data, ledm_display_t *dsp);
    void (*handleFrameTick)(ledm_display_t *dsp);
    void (*handleRowTick)(ledm_display_t *dsp);
} spr_routine_t;

// ====================== API Global Variables ==================

extern spr_routine_t *spr_currentRoutine;

// ========================== API methods =======================
#if defined(__cplusplus)
extern "C" {
#endif

void spr_loadRoutineNr(uint8_t nr);

#if defined(__cplusplus)
}
#endif

#endif /* SPACEROUTINES_H */
