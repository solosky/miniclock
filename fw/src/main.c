#include "fb.h"
#include "debug.h"

char s[] = "23:45";

void setup(){

  debug_init();

  fb_t* fb = fb_default();
  fb_init(fb);

  fb_draw_char(fb, '2', 0, 0, font_4x7_led);
  fb_draw_char(fb, '3', 5, 0, font_4x7_led);
  fb_draw_char(fb, ':', 10, 0, font_4x7_led);
  fb_draw_char(fb, '4', 12, 0, font_4x7_led);
   fb_draw_char(fb, '5', 17, 0, font_4x7_led);
//  fb_draw_string(fb, s, 0, 0, font_4x7_led);
  fb_flush(fb);
  fb_set_brightness(fb, 7);


}
void set_row(fb_t* fb, uint8_t row){
  for(uint8_t i=0; i<22; i++){
    _fb_set_pixel(fb, i, row, 1);
  }
}

void set_col(fb_t* fb, uint8_t col){
  for(uint8_t i=0; i<7; i++){
    _fb_set_pixel(fb, col, i, 1);
  }
}
char c = '0';
uint8_t a= 0;
void loop(){
    // fb_t* fb = fb_default();
    // fb_clear_display(fb);
    // //set_col(fb,a);
    // fb_draw_char(fb, c, 5, 0, font_4x7_led);
    // fb_flush(fb);
    // c++;
    // a++;
    // delay(1000);
    // if(a == 22){
    //   a = 0;
    // }
    // if(c == '9'){
    //   c = '0';
    // }
}
