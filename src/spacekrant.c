#include "spacekrant.h"

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
    // TODO: move this shit out of the way
    // binary 144bit counter
    // for(int r=LEDM_ROWS-1; r>=0; r--) {
    //     for(int c=LEDM_COLBYTES-1; c>=0; c--) {
    //         if(++display.buffer[r][c] != 0) return;
    //     }
    // }
    // delay(1);

    // TODO: move this shit out of the way
    // infinite scrolling up
    // static uint8_t i = 0;
    // clear();
    // dspm_writeString2Display("wat!\0", &display, i, 3);
    // dspm_writeString2Display("wat!\0", &display, i-7, 3);
    // delay(200);
    // i = (i+1) % 7;

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
    // TODO change routine
    dspm_clear(&display);
}

void serialDataHandler(uint8_t data) {
    // TODO forward this event to some routine
    dspm_fill(&display);
}

void onFrameFinished() {
    // TODO forward this event to some routine
}

void onRowFinished() {
    // TODO forward this event to some routine
}
