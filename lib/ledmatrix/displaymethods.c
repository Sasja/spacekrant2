#include "displaymethods.h"

// ========================== Constants =========================

// ============================ Macros ==========================

// ====================== Non-API Prototypes ====================

// ========================== Variables =========================
const uint8_t dspm_oneMasks[9] = {
    B00000000,B10000000,B11000000,B11100000,
    B11110000,B11111000,B11111100,B11111110,B11111111
};

// ======================== Implementation ======================


void dspm_clear(ledm_display_t *display){
    for(int row=0; row<LEDM_ROWS; row++) {
        for (int bCol=0; bCol<LEDM_COLBYTES; bCol++) {
            display->buffer[row][bCol] = 0;
        }
    }
}

void dspm_fill(ledm_display_t *display){
    for(int row=0; row<LEDM_ROWS; row++) {
        for (int bCol=0; bCol<LEDM_COLBYTES; bCol++) {
            display->buffer[row][bCol] = (uint8_t)-1;
        }
    }
};

uint8_t dspm_writeChar2Display(char c, ledm_display_t *display,
        int8_t rowOffset, int16_t colOffset)
{
    ltr_bitmap_t *bitmap = ltr_lookupBitmap(c);
    uint8_t width = ltr_getCharLength(bitmap);
    // loop over all lines of bitmap
    for(uint8_t charRow = 0; charRow<LEDM_ROWS; charRow++) {
        uint8_t dispRow = charRow + rowOffset;  
        // if line falls on the screen
        if(dispRow < LEDM_ROWS) { // counting on overflows for negative offsets
            //display->buffer[dispRow][0] = (*bitmap)[charRow] & B11111000;
            // now updateing some bits possibly spread over two display bytes
            int8_t colByteNr = colOffset / 8;
            int8_t colBits = colOffset % 8;
            if(colBits < 0) {   
                colBits += 8;
                colByteNr -= 1;
            }
            // the first byte to adapt
            if(colByteNr >= 0 && colByteNr < LEDM_COLBYTES) {
                uint8_t mask = dspm_oneMasks[width] >> colBits;
                display->buffer[dispRow][colByteNr] &= ~mask;
                display->buffer[dispRow][colByteNr] |= (*bitmap)[charRow] >> colBits & mask;
            }
            // the second byte to adapt
            if(colByteNr+1 >= 0 && colByteNr+1 < LEDM_COLBYTES && colBits != 0) {
                uint8_t mask = dspm_oneMasks[width] << 8 - colBits;
                display->buffer[dispRow][colByteNr+1] &= ~mask;
                display->buffer[dispRow][colByteNr+1] |= (*bitmap)[charRow] << (8 - colBits) & mask;
            }
        }
    }
    return width;
}

uint16_t dspm_writeString2Display(char str[], ledm_display_t *display,
        int8_t rowOffset, int16_t colOffset) {
    uint16_t width = 0;
    uint8_t strPos = 0;
    while(str[strPos] != '\0') { // && colOffset + width < LEDM_COLS) {
        width += dspm_writeChar2Display(str[strPos], display, rowOffset, colOffset + width);
        width++;  // add some space between chars
        strPos++;
    }
    return width;
}

void dspm_showBitmap(btm_bitmap_t *bitmap, ledm_display_t *display,
        int8_t rowOffset, int8_t colOffset) {
    // TODO offsets sideways
    for(uint8_t r = 0; r<LEDM_ROWS; r++) {
        for(uint8_t colb = 0; colb<LEDM_COLBYTES; colb++) {
            uint8_t bmrow = (r + rowOffset) % bitmap->height;
            uint8_t index = bmrow * bitmap->widthbytes + colb;
            display->buffer[r][colb] = 
                bitmap->buffer[index];
        }
    }
}
