#include "letters.h"

// ========================== Constants =========================

// ============================ Macros ==========================

// ====================== Non-API Prototypes ====================

// ========================== Variables =========================

// ======================== Implementation ======================


ltr_bitmap_t *ltr_lookupBitmap(char c) {
    if(c>='a' && c<='z') return &ltr_lowercase[c-'a'];
    if(c>='A' && c<='Z') return &ltr_uppercase[c-'A'];
    switch(c) {
        case ' ': return &ltr_space;
        case '`': return &ltr_space_small;
        case '.': return &ltr_dot;
        case ',': return &ltr_comma;
        case ':': return &ltr_colon;
        case ';': return &ltr_semicolon;
        case '!': return &ltr_exclamation;
        case '-': return &ltr_dash;
        case '@': return &ltr_at;
        case '\'':return &ltr_quote;
        default: return &ltr_space;
    }
}

uint8_t ltr_getCharLength(ltr_bitmap_t *b) {
    return (((*b)[3] & 1) << 2) | (((*b)[4] & 1) << 1) | ((*b)[5] & 1);
}
