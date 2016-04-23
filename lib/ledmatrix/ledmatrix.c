#include "ledmatrix.h"


// ========================== Constants =========================
#define LATCHPIN 10
#define CLOCKPIN 13
#define DATAPIN 11
#define CLOCK 9
#define RESET 8
#define LATCHPINNR 2

static const EMPTYLINE[LEDM_COLBYTES];

// ============================ Macros ==========================

// ====================== Non-API Prototypes ====================
static void setupSPI();
static void rowISR();
static void setupPins();

// ========================== Variables =========================
static ledm_display_t *currentDisplay = NULL;

// ======================== Implementation ======================
void ledm_setup() {
    setupPins();
    setupSPI();
    TimerOne_initialize(1000);
    TimerOne_attachInterrupt(rowISR);
}

void ledm_setDisp(ledm_display_t *display) {
    currentDisplay = display;
}

static void resetHwCounter() {
    digitalWrite(RESET, HIGH);
    digitalWrite(RESET, LOW);
}

static void incHwCounter() {
    digitalWrite(CLOCK, HIGH);
    digitalWrite(CLOCK, LOW);
}

static void latchOn() {
    bitSet(PORTB, LATCHPINNR);
}

static void latchOff() {
    bitClear(PORTB, LATCHPINNR);
}

static void pushLedRow(uint8_t data[LEDM_COLBYTES]) {
    latchOff();
    for(int i=0; i<LEDM_COLBYTES; i++) {
        SPDR = data[i];
        while (!(SPSR & (1<<SPIF))) {};
    }
    latchOn();
}

static void rowISR() {
    static uint8_t nextRow = 0;
    // clear the current ledlrow
    pushLedRow(EMPTYLINE);
    // enable the next row
    if(nextRow == 0) resetHwCounter();
        else incHwCounter();
    // light up the leds, increase the rowcount, set flag
    if(currentDisplay) {
        pushLedRow(currentDisplay->buffer[nextRow]);
        currentDisplay->rowFinishedFlag = true;
    }
    // shadow the hw row, and set flag after drawing the last row
    nextRow++;
    if (nextRow >= LEDM_ROWS) {
        nextRow = 0;
        if(currentDisplay) currentDisplay->frameFinishedFlag = true;
    }
}

static void setupSPI() {
    uint8_t clr;
    SPCR |= ( (1<<SPE) | (1<<MSTR) );   // enable SPI as master
    SPCR &= ~( (1<<SPR1) | (1<<SPR0) ); // clear prescaler bits
    SPSR |= (1<<SPI2X);                 // set prescaler bits
    delay(10);
}

static void setupPins() {
    pinMode(DATAPIN,OUTPUT);
    pinMode(CLOCKPIN,OUTPUT);
    pinMode(LATCHPIN,OUTPUT);
    pinMode(CLOCK,OUTPUT);
    pinMode(RESET,OUTPUT);
    digitalWrite(RESET,HIGH);
    digitalWrite(RESET,LOW);
}
