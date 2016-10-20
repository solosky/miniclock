#include "fb.h"
#include "font.h"

fb_t _fb;


fb_t* fb_default(){
  return &_fb;
}

void fb_init(fb_t* fb){
    tm_init(fb->tm[0], TM1_PIN_SCLK, TM1_PIN_DIN);
    tm_init(fb->tm[1], TM2_PIN_SCLK, TM1_PIN_DIN);
    memset(&fb->mem, 0, sizeof(fb->mem));
}


void fb_draw_char(fb_t* fb, char ch, uint8_t x, uint8_t y, font_t* font){
    glyph_t glyph;
    if(font_get_glyph(font, ch, &glyph) != ERR_GLYPH_NOT_FOUND){

    }
}


void fb_flush(fb_t* fb){
  byte_t buff[];
}
