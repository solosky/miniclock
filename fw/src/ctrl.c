#include "ctrl.h"
#include "debug.h"
#include "Arduino_FreeRTOS.h"
#include "timers.h"

ctrl_t _ctrl0;


ctrl_t* ctrl_default(){
  return &_ctrl0;
}


void ctrl_init(ctrl_t* ctrl){

    // debug init
    usart_init(usart_default(), DBG_USART_BAUD);
    debug_init(usart_default());

    //device i2c_init
    fb_init(fb_default());
    i2c_init(i2c_default());
    rtc_init(rtc_default(), i2c_default());
    dht_init(dht_default());

    key_init(key_default(), _ctrl_callback_on_key);
    lux_init(lux_default(), _ctrl_callback_on_lux);

    // create and start timers
    _ctrl_init_timers(ctrl);

}

void _ctrl_init_timers(ctrl_t* ctrl){
    _ctrl_new_timer("key_scan", 1, _ctrl_timer_key_scan);
    _ctrl_new_timer("read_lux", 150 / portTICK_PERIOD_MS,  _ctrl_timer_read_lux);
    _ctrl_new_timer("read_dht", 1000 / portTICK_PERIOD_MS, _ctrl_timer_read_dht);
    _ctrl_new_timer("flash_dot",500 / portTICK_PERIOD_MS, _ctrl_timer_flash_dot);
}

void _ctrl_new_timer(const char* name, uint16_t tick_count, timer_fn_t timer_fn){
  TimerHandle_t handle = xTimerCreate(name, tick_count, pdTRUE, (void *)0, timer_fn);
  xTimerStart( handle, 0 );
}

////////////////////////////////////////////////////////////////////////////////
void _ctrl_timer_key_scan(void* p){
  key_scan(key_default());
}

void _ctrl_timer_read_lux(void* p){
  lux_sample(lux_default());
}

void _ctrl_timer_flash_dot(void* p){

}

void _ctrl_timer_read_dht(void* p){
      result_t ret = dht_read(dht_default(), &(ctrl_default()->dht_data));
      if(ret != ERR_OK){
        printf("dht read error:%d\n", ret);
        return;
      }
}

////////////////////////////////////////////////////////////////////////////////

void _ctrl_callback_on_key(uint8_t key, uint8_t event){

}

void _ctrl_callback_on_lux(uint16_t lux){

}
