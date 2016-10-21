#include "fb.h"
#include "debug.h"
#include "rtc.h"
#include "Arduino_FreeRTOS.h"

char s[] = "23:45";


void setup(){

  debug_init();

  fb_t* fb = fb_default();
  fb_init(fb);

  fb_draw_string(fb, s, 0, 0, font_4x7_led);
  fb_flush(fb);
  fb_set_brightness(fb, 7);

  rtc_data_t rtc_data;
  rtc_t* rtc = rtc_default();
  rtc_init(rtc);
  rtc_read(rtc, &rtc_data);



  //printf("init done~\n");

    vTaskStartScheduler();
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
char dot = ':';
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
    rtc_data_t rtc_data;
    rtc_t* rtc = rtc_default();
    rtc_read(rtc, &rtc_data);

  //   printf("%.04d-%.02d-%.02d %.02d:%.02d:%.02d\n",
  //     rtc_data.year,
  //     rtc_data.month,
  //     rtc_data.day,
  //     rtc_data.hour,
  //     rtc_data.minute,
  //     rtc_data.second
  // );

  char time_str[6];
  sprintf(time_str, "%.02d%c%02d", rtc_data.hour, dot , rtc_data.minute);
  fb_t* fb = fb_default();
  fb_clear_display(fb);
  fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
  fb_flush(fb);
  fb_set_brightness(fb, 3);
  dot = dot ==':' ? ' ': ':';
  delay(500);
}
