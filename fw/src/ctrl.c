#include "ctrl.h"
#include "debug.h"
#include "timers.h"

ctrl_t _ctrl0;


ctrl_t* ctrl_default(){
  return &_ctrl0;
}


void ctrl_init(ctrl_t* ctrl){

    // debug init
    usart_init(usart_default(), DBG_USART_BAUD);
    debug_init(usart_default());

    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    //device i2c_init
    fb_init(fb_default());
    i2c_init(i2c_default());
    rtc_init(rtc_default(), i2c_default());
    dht_init(dht_default());
    view_init(view_default(), fb_default());

    key_init(key_default(), _ctrl_callback_on_key);
    lux_init(lux_default(), _ctrl_callback_on_lux);

    // create and start timers
    _ctrl_init_timers(ctrl);

    view_show_chain(view_default(), _view_chain_default);
    //view_show_page(view_default(), PG_TIME);
    //view_show_page(view_default(), PG_HUMIDITY);
    //view_show_page(view_default(), PG_TEMP);

}

void _ctrl_init_timers(ctrl_t* ctrl){
    timer_new("key_scan", 1, _ctrl_timer_key_scan, true);
    timer_new("read_lux", 150 / portTICK_PERIOD_MS,  _ctrl_timer_read_lux, true);
    timer_new("read_dht", 1000 / portTICK_PERIOD_MS, _ctrl_timer_read_dht, true);
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
      dht_data_t* dht_data = &(view_default()->view_data.dht_data);
      result_t ret = dht_read(dht_default(), dht_data);
      if(ret != ERR_OK){
        printf("dht read error:%d\n", ret);
        return;
      }
      //printf("dht: %d %d\n", dht_data->humidity, dht_data->temperature);
}

////////////////////////////////////////////////////////////////////////////////

void _ctrl_callback_on_key(uint8_t key, uint8_t event){

}

void _ctrl_callback_on_lux(uint16_t lux){

}
