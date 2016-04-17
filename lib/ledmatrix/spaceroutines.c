#include "spaceroutines.h"
#include "displaymethods.h"

#define TEXTBUFFERSIZE 64
// ========================== Constants =========================

// ============================ Macros ==========================

// ====================== Non-API Prototypes ====================

void textBuffer_handle(char data);
void textBuffer_clear();

void nopData(uint8_t, ledm_display_t*);
void nop(ledm_display_t*);

void counter144Tick(ledm_display_t*);

void whatsupFrame(ledm_display_t*);

void textScrollerInit(ledm_display_t*);
void textScrollerData(uint8_t, ledm_display_t*);
void textScrollerFrame(ledm_display_t*);

// ========================== Variables =========================

char textBuffer[TEXTBUFFERSIZE] = {'\0'};
uint8_t textBuffer_pos = 0;

// i'll put this here as a reference...
//
// typedef struct spr_routine {
//     void (*init)(ledm_display_t *dsp);
//     void (*handleData)(uint8_t data, ledm_display_t *dsp);
//     void (*handleFrameTick)(ledm_display_t *dsp);
//     void (*handleRowTick)(ledm_display_t *dsp);
// } spr_routine_t;

spr_routine_t nopRoutine = {
    .init = nop,
    .handleData = nopData,
    .handleFrameTick = nop,
    .handleRowTick = nop
};

spr_routine_t fillRoutine = {
    .init = dspm_fill,
    .handleData = nopData,
    .handleFrameTick = nop,
    .handleRowTick = nop
};

spr_routine_t clearRoutine = {
    .init = dspm_clear,
    .handleData = nopData,
    .handleFrameTick = nop,
    .handleRowTick = nop
};

spr_routine_t counter144Routine = {
    .init = dspm_clear,
    .handleData = nopData,
    .handleFrameTick = nop,
    .handleRowTick = counter144Tick
};

spr_routine_t whatsupRoutine = {
    .init = dspm_clear,
    .handleData = nopData,
    .handleFrameTick = whatsupFrame,
    .handleRowTick = nop
};

spr_routine_t textScrollerRoutine = {
    .init = textScrollerInit,
    .handleData = textScrollerData,
    .handleFrameTick = textScrollerFrame,
    .handleRowTick = nop
};

spr_routine_t *routines[] = {
    &nopRoutine,            // a
    &fillRoutine,           // b
    &clearRoutine,          // c
    &counter144Routine,     // d
    &whatsupRoutine,        // e
    &textScrollerRoutine,   // f
};

spr_routine_t *spr_currentRoutine = &nopRoutine;

// ======================== Implementation ======================

void spr_loadRoutineNr(uint8_t nr) {
    if(nr < sizeof(routines)/sizeof(routines[0])) {
        spr_currentRoutine = routines[nr];
    } else {
        spr_currentRoutine = routines[0];
    }
}

void textBuffer_handle(char data) {
    if(data == 0x7 || data == 0x7f) { // del or backspace
        if(textBuffer_pos > 0) {
            textBuffer[textBuffer_pos--] = '\0';
            textBuffer[textBuffer_pos] = data;
        }
    } else {
        if(textBuffer_pos < TEXTBUFFERSIZE-1) { // theres room for a '\0'
            textBuffer[textBuffer_pos++] = data;
            textBuffer[textBuffer_pos] = '\0';
        }
    }
}

void textBuffer_clear() {
    textBuffer_pos = 0;
    textBuffer[0] = '\0';
}

// ===============================================

void nopData(uint8_t data, ledm_display_t *dsp) {}
void nop(ledm_display_t *dsp) {}

void counter144Tick(ledm_display_t* dsp) {
    for(int r=LEDM_ROWS-1; r>=0; r--) {
        for(int c=LEDM_COLBYTES-1; c>=0; c--) {
            if(++dsp->buffer[r][c] != 0) return;
        }
    }
}

// ===============================================

void whatsupFrame(ledm_display_t* dsp) {
    static uint8_t prescaler;
    prescaler = (prescaler + 1) % 20;
    if(prescaler) return;

    static uint8_t i = 0;
    dspm_writeString2Display("wat!\0", dsp, i, 3);
    dspm_writeString2Display("wat!\0", dsp, i-7, 3);
    i = (i+1) % 7;
}

// ===============================================

int16_t textScroller_width = 0;
int16_t textScroller_offset = 0;

void textScroller_update(ledm_display_t *dsp) {
    dspm_clear(dsp);
    textScroller_width = dspm_writeString2Display(
            textBuffer,
            dsp,
            0,
            textScroller_offset);
}

void textScrollerInit(ledm_display_t *dsp) {
    textBuffer_clear();
    textScroller_update(dsp);
}

void textScrollerData(uint8_t data, ledm_display_t *dsp) {
    textBuffer_handle(data);
    textScroller_update(dsp);
}

void textScrollerFrame(ledm_display_t *dsp) {
    static uint8_t prescaler;
    prescaler = (prescaler + 1) % 8;
    if(prescaler) return;
    
    textScroller_offset--;
    if(textScroller_offset + textScroller_width <= 0)
        textScroller_offset = LEDM_COLS - 1;

    textScroller_update(dsp);
}
