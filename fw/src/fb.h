#ifndef FB_H
#define FB_H

#include "base.h"
#include "tm.h"
#include "font.h"


#define FB_PIXEL_W  22
#define FB_PIXEL_H  7
#define FB_MEM_W 3
#define FB_MEM_H 7

#define TM1_PIN_SCLK 5
#define TM1_PIN_DIN 4
#define TM2_PIN_SCLK 7
#define TM2_PIN_DIN 6

#define TM_DATA_SIZE 16
#define TM_BANK_SIZE 2
#define TM_BANK_ROW 11


typedef struct _fb_ {
    uint8_t mem[FB_MEM_H][FB_MEM_W];  //mem mapping, 1bit per dot
    tm_t tm[TM_BANK_SIZE];    // two tm1640 drivers
} fb_t;



////////////////////////////////////////////////////////////////
// public functions
////////////////////////////////////////////////////////////////
fb_t* fb_default();
void fb_init(fb_t* fb);
void fb_clear_display(fb_t* fb);
uint8_t fb_draw_char(fb_t* fb, char ch, uint8_t x, uint8_t y, font_t* font);
uint8_t fb_draw_string(fb_t* fb, char* str, uint8_t x, uint8_t y, font_t* font);
void fb_set_brightness(fb_t* fb, uint8_t level);
void fb_flush(fb_t* fb);


///////////////////////////////////////////////////////////////
// private functions
///////////////////////////////////////////////////////////////
void _fb_set_pixel(fb_t* fb, uint8_t x, uint8_t y, bool_t pixel);
bool_t _fb_get_pixel(fb_t* fb, uint8_t x, uint8_t y);
void _fb_flush_bank(fb_t* fb, uint8_t bank);


#endif
