#include "Serial_C.h"

// ========================== Constants =========================

// ============================ Macros ==========================

// ====================== Non-API Prototypes ====================

// ========================== Variables =========================

// ======================== Implementation ======================

void Serial_begin(uint16_t baud) {
    Serial.begin(baud);
}

bool Serial_available() {
    return Serial.available();
}


int16_t Serial_read() {
    return Serial.read();
}
