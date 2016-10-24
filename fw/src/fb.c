#include "fb.h"
#include "font.h"
#include "tm.h"

fb_t _fb;


void _fb_set_pixel(fb_t* fb, uint8_t x, uint8_t y, bool_t pixel){
        if(x >FB_PIXEL_W || y > FB_PIXEL_H) {
                return;
        }

        uint8_t xi = x >> 3; // div 8, x index
        uint8_t xo = x - ( xi << 3);  // x offset
        uint8_t v = fb->mem[y][xi];

        v &= ~(1 << xo);        //clear bit
        v |= (pixel << xo);    //set but

        fb->mem[y][xi] = v;
}

bool_t _fb_get_pixel(fb_t* fb, uint8_t x, uint8_t y){
        if(x >FB_PIXEL_W || y > FB_PIXEL_H) {
                return 0;
        }
        uint8_t xi = x >> 3; // div 8, x index
        uint8_t xo = x - (xi << 3);  // x offset
        uint8_t v = fb->mem[y][xi];
        return ( v >> xo ) & 1;
}

void _fb_flush_bank(fb_t* fb, uint8_t bank){
        uint8_t mem[TM_DATA_SIZE] = {0};
        for(uint8_t x= 0; x< TM_BANK_ROW; x++) {
                uint8_t v = 0;
                for(uint8_t y = 0; y<FB_PIXEL_H; y++) {
                        bool_t b = _fb_get_pixel(fb, bank*TM_BANK_ROW + x, y);
                        v |= b << y;
                }
                mem[x] = v;
        }

        tm_display(&fb->tm[bank], mem, sizeof(mem));
}



fb_t* fb_default(){
        return &_fb;
}


void fb_init(fb_t* fb){
        tm_init(&fb->tm[0], TM1_PIN_SCLK, TM1_PIN_DIN);
        tm_init(&fb->tm[1], TM2_PIN_SCLK, TM2_PIN_DIN);
        memset(&fb->mem, 0, sizeof(fb->mem));
}

void fb_set_brightness(fb_t* fb, uint8_t level){
        tm_set_brightless(&fb->tm[0], level);
        tm_set_brightless(&fb->tm[1], level);
}

void fb_clear_display(fb_t* fb){
        memset(&fb->mem, 0, sizeof(fb->mem));
}


uint8_t fb_draw_char(fb_t* fb, char ch, uint8_t x, uint8_t y,const font_t* font){
        if(x > FB_PIXEL_W || y > FB_PIXEL_H) {
                return x;
        }

        glyph_t glyph;
        if(font_get_glyph(font, ch, &glyph) != ERR_GLYPH_NOT_FOUND) {
                for(uint8_t yy = 0; yy < min(glyph.h, FB_PIXEL_H - y); yy++) {
                        uint8_t b = glyph.bitmap[yy];
                        for(uint8_t xx = 0; xx < min(glyph.w, FB_PIXEL_W - x); xx++) {
                                _fb_set_pixel(fb, x + xx, y + yy, (b >> (7 - xx )) & 1);
                        }
                }
                return x + glyph.w + 1;
        }
        return x;
}


uint8_t fb_draw_string(fb_t* fb, char* str, uint8_t x, uint8_t y, const font_t* font){
        for(char* p=str; *p != 0; p++) {
                x = fb_draw_char(fb, *p, x, y, font);
        }
        return x;
}

void fb_flush(fb_t* fb){
        _fb_flush_bank(fb, 0);
        _fb_flush_bank(fb, 1);
}
