#include "spaceroutines.h"
#include "displaymethods.h"

#define TEXTBUFFERSIZE 64
// ========================== Constants =========================

// ============================ Macros ==========================

// ====================== Non-API Prototypes ====================
// ---------------------- helper methods ------------------------

void textBuffer_handle(char data);
void textBuffer_clear();

void wrapRight(uint8_t buffer[LEDM_ROWS][LEDM_COLBYTES]);
void wrapLeft(uint8_t buffer[LEDM_ROWS][LEDM_COLBYTES]);
void wrapUp(uint8_t buffer[LEDM_ROWS][LEDM_COLBYTES]);
void wrapDown(uint8_t buffer[LEDM_ROWS][LEDM_COLBYTES]);

// ---------------------- for the routines ----------------------

void nopData(uint8_t, ledm_display_t*);
void nop(ledm_display_t*);

void counter144Tick(ledm_display_t*);

void whatsupFrame(ledm_display_t*);

void textScrollerInit(ledm_display_t*);
void textScrollerData(uint8_t, ledm_display_t*);
void textScrollerFrame(ledm_display_t*);

void conwayInit(ledm_display_t*);
void conwayData(uint8_t, ledm_display_t*);
void conwayFrameTick(ledm_display_t*);
// ========================== Variables =========================

char textBuffer[TEXTBUFFERSIZE] = {'\0'};
uint8_t textBuffer_pos = 0;

// i'll put this here as a reference ;)
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

spr_routine_t conwayRoutine = {
    .init = conwayInit,
    .handleData = conwayData,
    .handleFrameTick = conwayFrameTick,
    .handleRowTick = nop,
};

spr_routine_t *routines[] = {
    &nopRoutine,            // a
    &fillRoutine,           // b
    &clearRoutine,          // c
    &counter144Routine,     // d
    &whatsupRoutine,        // e
    &textScrollerRoutine,   // f
    &conwayRoutine,         // g
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

// --------------- textBuffer -------------------

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

// --------------- shifting V & H ----------------

inline void wrapRight(uint8_t buffer[LEDM_ROWS][LEDM_COLBYTES]) {
    for(uint8_t row = 0; row < LEDM_ROWS; row++) {
        bool lastCarry = false;
        for(uint8_t colByte = 0; colByte < LEDM_COLBYTES; colByte++) {
            bool newCarry = buffer[row][colByte] & 0x1;
            buffer[row][colByte] >>= 1;
            if(lastCarry) buffer[row][colByte] |= 1<<7;
            lastCarry = newCarry;
        }
        if(lastCarry) buffer[row][0] |= 1<<7;
    }
}

inline void wrapLeft(uint8_t buffer[LEDM_ROWS][LEDM_COLBYTES]) {
    for(uint8_t row = 0; row < LEDM_ROWS; row++) {
        bool lastCarry = false;
        for(int8_t colByte = LEDM_COLBYTES-1; colByte >= 0; colByte--) {
            bool newCarry = buffer[row][colByte] & 1<<7;
            buffer[row][colByte] <<= 1;
            if(lastCarry) buffer[row][colByte] |= 0x1;
            lastCarry = newCarry;
        }
        if(lastCarry) buffer[row][LEDM_COLBYTES-1] |= 0x1;
    }
}

inline void wrapUp(uint8_t buffer[LEDM_ROWS][LEDM_COLBYTES]) {
    for(uint8_t colByte = 0; colByte < LEDM_COLBYTES; colByte++) {
        uint8_t firstRow = buffer[0][colByte];
        for(uint8_t row = 0; row < LEDM_ROWS-1; row++) {
            buffer[row][colByte] = buffer[row+1][colByte];
        }
        buffer[LEDM_ROWS-1][colByte] = firstRow;
    }
}

inline void wrapDown(uint8_t buffer[LEDM_ROWS][LEDM_COLBYTES]) {
    for(uint8_t colByte = 0; colByte < LEDM_COLBYTES; colByte++) {
        uint8_t lastRow = buffer[LEDM_ROWS-1][colByte];
        for(int8_t row = LEDM_ROWS-1; row >= 1; row--) {
            buffer[row][colByte] = buffer[row-1][colByte];
        }
        buffer[0][colByte] = lastRow;
    }
}

// =================counter144====================

void nopData(uint8_t data, ledm_display_t *dsp) {}
void nop(ledm_display_t *dsp) {}

void counter144Tick(ledm_display_t* dsp) {
    for(int r=LEDM_ROWS-1; r>=0; r--) {
        for(int c=LEDM_COLBYTES-1; c>=0; c--) {
            if(++dsp->buffer[r][c] != 0) return;
        }
    }
}

// ===============whatsup=========================

void whatsupFrame(ledm_display_t* dsp) {
    static uint8_t prescaler;
    prescaler = (prescaler + 1) % 20;
    if(prescaler) return;

    static uint8_t i = 0;
    dspm_writeString2Display("wat!\0", dsp, i, 3);
    dspm_writeString2Display("wat!\0", dsp, i-7, 3);
    i = (i+1) % 7;
}

// =============textScroller======================

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

// ============== Conways game of life ===========

uint8_t conwayBuffer[LEDM_ROWS][LEDM_COLS];
uint8_t conwayDispBuffer[LEDM_ROWS][LEDM_COLBYTES];
uint8_t conwaySerialData;
bool conwayNewSerialDataAvailable = false;
uint8_t conwayPrescaler = 16;

inline void conwayClearBuff() {
    memset(conwayBuffer, 0, sizeof(conwayBuffer[0][0])*LEDM_ROWS*LEDM_COLS);
}


inline void conwayAccum(uint8_t buffer[LEDM_ROWS][LEDM_COLBYTES], const int8_t weight) {
    for(uint8_t row = 0; row < LEDM_ROWS; row++) {
        for(uint8_t colByte = 0; colByte < LEDM_COLBYTES; colByte++) {
            uint8_t bits = buffer[row][colByte];
            for(uint8_t i = 0; i<8; i++) {
                if(bits & 1<<7) conwayBuffer[row][(colByte << 3) + i] += weight;
                bits <<= 1;
            }
        }
    }
}

void conwayBuff2Disp(ledm_display_t *dsp) {
    for(uint8_t row = 0; row < LEDM_ROWS; row++) {
        for(uint8_t colByte = 0; colByte < LEDM_COLBYTES; colByte++) {
            uint8_t bits = 0;
            for(int8_t i = 0; i<8; i++) {
                bits <<= 1;
                uint8_t sum = conwayBuffer[row][(colByte << 3) + i];
                if(sum == 18 || sum == 19 || sum == 3) bits++;
            }
            dsp->buffer[row][colByte] = bits;
        }
    }
}

void conwayInit(ledm_display_t *dsp) {
    // do nothing, just iterate on display content
}

void conwayData(uint8_t data, ledm_display_t *dsp) {
    // dont just draw here or it might be overwitten with results before taken into account.
    static bool escaped = false;
    if(escaped) {
        escaped = false;
        conwayPrescaler = 2 + 3 * (data - 'a');
    } else {
        if (data == 0x1b) {                         // a first ESC
            escaped = true;
        } else {
            conwaySerialData = data;
            conwayNewSerialDataAvailable = true;
        }
    }
}

void conwayFrameTick(ledm_display_t *dsp) {
    static bool newBufferReady = false;
    if(newBufferReady) {
        newBufferReady = false;
        conwayBuff2Disp(dsp);               // calculate and draw result
    }

    // new chars as seed are drawn after the update to prevent them being lost in calculation
    // this might cause some delay
    if(conwayNewSerialDataAvailable) {
        conwayNewSerialDataAvailable = false;
        uint8_t width = ltr_getCharLength(ltr_lookupBitmap(conwaySerialData));
        dspm_writeChar2Display(conwaySerialData, dsp, 0, (LEDM_COLS - width)/2);   // some input to feed conway
    }
    
    static uint8_t pcnt;
    pcnt = (pcnt + 1) % conwayPrescaler;
    if(pcnt) return;

    memcpy(conwayDispBuffer, dsp->buffer, sizeof(conwayBuffer[0][0])*LEDM_ROWS*LEDM_COLS);
    
    conwayClearBuff();
    conwayAccum(conwayDispBuffer, 16);       // 5 on keypad
    wrapRight(conwayDispBuffer);
    conwayAccum(conwayDispBuffer, 1);        // 6
    wrapDown(conwayDispBuffer);
    conwayAccum(conwayDispBuffer, 1);        // 3
    wrapLeft(conwayDispBuffer);
    conwayAccum(conwayDispBuffer, 1);        // 2
    wrapLeft(conwayDispBuffer);
    conwayAccum(conwayDispBuffer, 1);        // 1
    wrapUp(conwayDispBuffer);
    conwayAccum(conwayDispBuffer, 1);        // 4
    wrapUp(conwayDispBuffer);
    conwayAccum(conwayDispBuffer, 1);        // 7
    wrapRight(conwayDispBuffer);
    conwayAccum(conwayDispBuffer, 1);        // 8
    wrapRight(conwayDispBuffer);
    conwayAccum(conwayDispBuffer, 1);        // 9
    // some lines of the next frame will be drawn by now, so wait for new frametick to update display
    newBufferReady = true;      // conwayBuffer is ready to be processed and displayed
}
