#include "spacekrant.h"
#include "spaceroutines.h"

// ========================== Constants =========================

// ============================ Macros ==========================

// ====================== Non-API Prototypes ====================
void serialDataHandler(uint8_t data);
void serialEscapedHandler(uint8_t data);
void onFrameFinished();
void onRowFinished();

// ======================= Global Variables =====================
ledm_display_t display;

// ======================== Implementation ======================

void setup() {
    Serial_begin(9600);
    ledm_setup();
    ledm_setDisp(&display);
}

void loop() {
    if(display.frameFinishedFlag) {
        display.frameFinishedFlag = false;
        onFrameFinished();
    }
    if(display.rowFinishedFlag) {
        display.rowFinishedFlag = false;
        onRowFinished();
    }
    if (Serial_available()) onSerial();
}

void onSerial() {
    static bool escaped = false;
    int16_t data;
    while((data = Serial_read()) != -1 ) {
        if(escaped) {
            escaped = false;
            if (data == 0x1b) {                         // ESC + ESC
                serialDataHandler(data);                // sending an ESC as data
            } else {                                    // this is a regular escaped byte
                serialEscapedHandler(data);
            }
        } else {
            if (data == 0x1b) {                         // a first ESC
                escaped = true;
            } else {
                serialDataHandler(data);                // regular data
            }
        }
    }
}

void serialEscapedHandler(uint8_t data) {
    spr_loadRoutineNr(data - 'a');
    spr_currentRoutine->init(&display);
}

void serialDataHandler(uint8_t data) {
    spr_currentRoutine->handleData(data, &display);
}

void onFrameFinished() {
    spr_currentRoutine->handleFrameTick(&display);
}

void onRowFinished() {
    spr_currentRoutine->handleRowTick(&display);
}
