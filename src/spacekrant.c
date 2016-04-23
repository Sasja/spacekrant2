#include <Arduino.h>
#include <stdlib.h>
#include "Serial_C.h"
#include "spacekrant.h"
#include "spaceroutines.h"
#include "ledmatrix.h"

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
    //randomSeed(analogRead(0));
    srand(analogRead(0) << 8 + analogRead(0));
    Serial_begin(9600);
    ledm_setup();
    ledm_setDisp(&display);
    spr_currentRoutine->init(&display);
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

static bool noSerialInput = true;
void serialDataHandler(uint8_t data) {
    noSerialInput = false;
    spr_currentRoutine->handleData(data, &display);
}

static int16_t secCnt = 0;
void onFrameFinished() {
    spr_currentRoutine->handleFrameTick(&display);
    if(++secCnt >= 1000/6) secCnt = 0;
    if(secCnt == 0) onSecond();
}

void onRowFinished() {
    spr_currentRoutine->handleRowTick(&display);
}

// this is just a routine to do something if theres no serial input
void onSecond() {
    static uint16_t cnt = 0;
    if(!noSerialInput) return;

    if(cnt == 0) serialEscapedHandler('h');
    if(cnt == 10) serialEscapedHandler('g');
    if(cnt > 10 && cnt % 15 == 0) {
        uint8_t letter = 'a' + rand()%26;
        spr_currentRoutine->handleData(letter, &display);
    }

    if(++cnt > 500) cnt = 0;
}
