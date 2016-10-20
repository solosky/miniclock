#ifndef FB_H
#define FB_H


#define FB_PIXEL_W  22
#define FB_PIXEL_H  7
#define FB_MEM_W 3
#define FB_MEM_H 7
#define FB_TM_SIZE 2

#define TM1_PIN_SCLK 4
#define TM1_PIN_DIN 5
#define TM2_PIN_SCLK 6
#define TM2_PIN_DIN 7

#define TM_DATA_SIZE 16

typedef uint8_t ret_t

typedef struct _fb_{
    uint8_t mem[FB_MEM_H][FB_MEM_W];  //mem mapping, 1bit per dot
    tm_t tm[FB_TM_SIZE];    // two tm1640 drivers
} fb_t;



////////////////////////////////////////////////////////////////
// public functions
////////////////////////////////////////////////////////////////
fb_t* fb_default();
void fb_init(fb_t* fb);
void fb_clear_display(fb_t* fb);
void fb_draw_char(fb_t* fb, char ch, uint8_t x, uint8_t y, font_t* font);
void fb_flush(fb_t* fb);


///////////////////////////////////////////////////////////////
// private functions
///////////////////////////////////////////////////////////////
void _fb_fill_tm_data(fb_t* fb, uint8_t idx, byte_t* buff, uint8_t size);
void _fb_write_tm_data(fb_t* fb, uint8_t idx, byte_t* buff, uint8_t size);


#endif
