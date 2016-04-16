#ifndef SERIAL_C_H
#define SERIAL_C_H

#include <Arduino.h>
#include <stdbool.h>

// =========================== Constants ========================

// ============================ Macros ==========================

// ========================= API Typedefs =======================

// ====================== API Global Variables ==================
// extern uint8_t tmpl_someApiVariable;

// ========================== API methods =======================
#if defined(__cplusplus)
extern "C" {
#endif

void Serial_begin(uint16_t baud);
bool Serial_available();
int16_t Serial_read();

#if defined(__cplusplus)
}
#endif

#endif /* SERIAL_C_H */
