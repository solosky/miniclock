#include "fb.h"
#include "debug.h"
#include "rtc.h"
#include "key.h"
#include "Arduino_FreeRTOS.h"
#include "timers.h"
#include "dht.h"
#include "lux.h"

#include "ctrl.h"

char s[] = "23:45";


// void key_scan_task(void* p){
//   while (true) {
//     key_scan(key_default());
//     vTaskY
//   }
// }



// uint8_t count = 0;
// void on_key(uint8_t key, uint8_t event){
//         char time_str[10];
//         //printf("K%d E%d\n", key, event);
//
//         if(event == KEY_EVENT_KEYDOWN) {
//                 sprintf(time_str, "%.03d", count++);
//                 fb_t* fb = fb_default();
//                 fb_clear_display(fb);
//                 fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
//                 fb_flush(fb);
//         }
// }
//
// void scan_key_timer(void* p){
//         key_scan(key_default());
// }
//
// void on_lux(uint16_t lux){
//     printf("lux: %d\n",lux);
//     fb_t* fb = fb_default();
//     if(lux < 100){
//       fb_set_brightness(fb, 7);
//     }else if(lux < 150){
//       fb_set_brightness(fb, 6);
//     }else if(lux < 200){
//       fb_set_brightness(fb, 5);
//     }else if(lux < 250){
//       fb_set_brightness(fb, 4);
//   }else if(lux < 300){
//       fb_set_brightness(fb, 3);
//     }else if(lux < 350){
//       fb_set_brightness(fb, 2);
//   }else if(lux < 400){
//       fb_set_brightness(fb, 1);
//   }else{
//       fb_set_brightness(fb, 0);
//   }
// }
//
// void read_lux_timer(void*p){
//     lux_sample(lux_default());
// }
//
//
//
// char dot = ':';
// dht_data_t dht_data = {0};
// uint8_t show_type = 0;
// uint8_t rotate_cnt = 0;
// void flush_time_timer(void* p){
//       char time_str[6];
//       zero(time_str, sizeof(time_str));
//           fb_t* fb = fb_default();
//           fb_clear_display(fb);
//         if(show_type == 0){
//           rtc_time_t * tm = &(rtc_default()->rtc_time);
//           sprintf(time_str, "%.02d%c%02d", tm->hour, dot, tm->minute);
//           dot = dot ==':' ? ' ' : ':';
//           fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
//       }else if(show_type ==1){
//           sprintf(time_str, "%.02d%%", dht_data.humidity);
//           fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
//       }else{
//           sprintf(time_str, "%.02d", dht_data.temperature);
//           time_str[ strlen(time_str)] = 0xE0;
//           fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
//       }
//           fb_flush(fb);
//
//       rotate_cnt ++;
//       if(rotate_cnt == 10){
//         rotate_cnt = 0;
//         if(show_type ==2){
//           show_type = 0;
//         }else{
//           show_type++;
//         }
//       }
// }
//
// void read_dht_timer(void*p){
//     result_t ret = dht_read(dht_default(), &dht_data);
//     if(ret != ERR_OK){
//       printf("dht read error:%d\n", ret);
//       return;
//     }
//     //printf("dht: %02d%%, %02dC\n", dht_data.humidity, dht_data.temperature);
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

void on_lux(uint16_t lux){
    printf("lux: %d\n",lux);
    fb_t* fb = fb_default();
    if(lux < 100){
      fb_set_brightness(fb, 7);
    }else if(lux < 150){
      fb_set_brightness(fb, 6);
    }else if(lux < 200){
      fb_set_brightness(fb, 5);
    }else if(lux < 250){
      fb_set_brightness(fb, 4);
  }else if(lux < 300){
      fb_set_brightness(fb, 3);
    }else if(lux < 350){
      fb_set_brightness(fb, 2);
  }else if(lux < 400){
      fb_set_brightness(fb, 1);
  }else{
      fb_set_brightness(fb, 0);
  }
}

void read_lux_timer(void*p){
    lux_sample(lux_default());
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
          uint8_t humidity_1 = dht_data.humidity / 10;
          uint8_t humidity_2 = dht_data.humidity - humidity_1 * 10;
          sprintf(time_str, "%.02d.%.1d%%", humidity_1, humidity_2);
          fb_draw_string(fb, time_str, 0, 0, font_4x7_led);
      }else{
          uint8_t temp_1 = dht_data.temperature / 10;
          uint8_t temp_2 = dht_data.temperature - temp_1 * 10;
          sprintf(time_str, "%.02d.%.1d", temp_1, temp_2);
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

      ctrl_init(ctrl_default());

        // debug_init();
        //
        // fb_init(fb_default());
        // rtc_init(rtc_default());
        // dht_init(dht_default());
        //
        // key_init(key_default(), on_key);
        // lux_init(lux_default(), on_lux);
        //
        // TimerHandle_t handle = xTimerCreate("KeyScan", 1, pdTRUE, (void *)0, scan_key_timer);
        // xTimerStart( handle, 0 );
        //
        // TimerHandle_t handle2 = xTimerCreate("FlushTime", 500 / portTICK_PERIOD_MS, pdTRUE, (void *)0, flush_time_timer);
        // xTimerStart( handle2, 0 );
        //
        //
        // TimerHandle_t handle3 = xTimerCreate("ReadDHT", 1000 / portTICK_PERIOD_MS, pdTRUE, (void *)0, read_dht_timer);
        // xTimerStart( handle3, 0 );
        //
        // TimerHandle_t handle4 = xTimerCreate("ReadLUX", 150 / portTICK_PERIOD_MS, pdTRUE, (void *)0, read_lux_timer);
        // xTimerStart( handle4, 0 );
        //
        //
        // pinMode(9, OUTPUT);



}

void loop(){

}
