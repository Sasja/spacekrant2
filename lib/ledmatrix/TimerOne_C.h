#ifndef TIMER_ONE_C_H
#define TIMER_ONE_C_H

// =========================== Constants ========================

// ============================ Macros ==========================

// ========================= API Typedefs =======================

// ====================== API Global Variables ==================
// extern uint8_t tmpl_someApiVariable;

// ========================== API methods =======================
#if defined(__cplusplus)
#include "TimerOne.h"
extern "C" {
#endif

void TimerOne_initialize(unsigned long microseconds);
void TimerOne_attachInterrupt(void (*isr)());

#if defined(__cplusplus)
}
#endif

#endif /* TIMER_ONE_C_H */
