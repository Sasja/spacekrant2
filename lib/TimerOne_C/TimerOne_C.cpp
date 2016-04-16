#include "TimerOne_C.h"

// ========================== Constants =========================

// ============================ Macros ==========================

// ====================== Non-API Prototypes ====================

// ========================== Variables =========================

// ======================== Implementation ======================

void TimerOne_initialize(unsigned long microseconds) {
    Timer1.initialize(microseconds);
}

void TimerOne_attachInterrupt(void (*isr)()) {
    Timer1.attachInterrupt(isr);
}
