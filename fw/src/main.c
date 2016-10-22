#include "fb.h"
#include "debug.h"
#include "rtc.h"
#include "key.h"
#include "Arduino_FreeRTOS.h"
#include "timers.h"
#include "dht.h"

char s[] = "23:45";


// void key_scan_task(void* p){
//   while (true) {
//     key_scan(key_default());
//     vTaskY
//   }
// }



uint8_t count = 0;
void on_key(uint8_t key, uint8_t event){
        char time_str[10];
        //printf("K%d E%d\n", key, event);

        if(event == KEY_EVENT_KEYDOWN) {
                sprintf(time_str, "%.03d", count++);
                fb_t* fb = fb_default();
                fb_clear_display(fb);
                fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
                fb_flush(fb);
        }
}

void scan_key_timer(void* p){
        key_scan(key_default());
}



char dot = ':';
dht_data_t dht_data = {0};
uint8_t show_type = 0;
uint8_t rotate_cnt = 0;
void flush_time_timer(void* p){
      char time_str[6];
      zero(time_str, sizeof(time_str));
          fb_t* fb = fb_default();
          fb_clear_display(fb);
        if(show_type == 0){
          rtc_time_t * tm = &(rtc_default()->rtc_time);
          sprintf(time_str, "%.02d%c%02d", tm->hour, dot, tm->minute);
          dot = dot ==':' ? ' ' : ':';
          fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
      }else if(show_type ==1){
          sprintf(time_str, "%.02d%%", dht_data.humidity);
          fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
      }else{
          sprintf(time_str, "%.02d", dht_data.temperature);
          time_str[ strlen(time_str)] = 0xE0;
          fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
      }
          fb_flush(fb);

      rotate_cnt ++;
      if(rotate_cnt == 10){
        rotate_cnt = 0;
        if(show_type ==2){
          show_type = 0;
        }else{
          show_type++;
        }
      }
}

void read_dht_timer(void*p){
    result_t ret = dht_read(dht_default(), &dht_data);
    if(ret != ERR_OK){
      printf("dht read error:%d\n", ret);
      return;
    }
    //printf("dht: %02d%%, %02dC\n", dht_data.humidity, dht_data.temperature);
}

void setup(){

        debug_init();

        fb_init(fb_default());
        rtc_init(rtc_default());
        dht_init(dht_default());

        key_init(key_default(), on_key);

        TimerHandle_t handle = xTimerCreate("KeyScan", 1, pdTRUE, (void *)0, scan_key_timer);
        xTimerStart( handle, 0 );

        TimerHandle_t handle2 = xTimerCreate("FlushTime", 500 / portTICK_PERIOD_MS, pdTRUE, (void *)0, flush_time_timer);
        xTimerStart( handle2, 0 );


        TimerHandle_t handle3 = xTimerCreate("ReadDHT", 1000 / portTICK_PERIOD_MS, pdTRUE, (void *)0, read_dht_timer);
        xTimerStart( handle3, 0 );

        pinMode(9, OUTPUT);

}



char c = '0';
uint8_t a= 0;
void loop(){
        rtc_date_time_t rtc_data;
        rtc_t* rtc = rtc_default();
        // rtc_read(rtc, &rtc_data);

        //   printf("%.04d-%.02d-%.02d %.02d:%.02d:%.02d\n",
        //     rtc_data.year,
        //     rtc_data.month,
        //     rtc_data.day,
        //     rtc_data.hour,
        //     rtc_data.minute,
        //     rtc_data.second
        // );

        char time_str[6];
        //sprintf(time_str, "%.02d%c%02d", rtc_data.hour, dot , rtc_data.minute);
        rtc_time_t * tm = &(rtc->rtc_time);
        //sprintf(time_str, "%.02d%c%02d", tm->minute, dot, tm->second);

        // int16_t val = analogRead(A2 );
        // sprintf( time_str, "%.03d", val);
        //
        // fb_t* fb = fb_default();
        // fb_clear_display(fb);
        // fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
        // fb_flush(fb);
        // fb_set_brightness(fb, 7);
        // dot = dot ==':' ? ' ' : ':';
        // delay(100);
        //key_scan(key_default());
        //delay(15);

}
