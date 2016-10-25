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
        timer_new("key_scan", KEY_SCAN_PEROD_MS / portTICK_PERIOD_MS, _ctrl_timer_key_scan, true);
        timer_new("view_tick", VIEW_TICK_PEROD_MS / portTICK_PERIOD_MS, _ctrl_timer_view_tick, true);
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

void _ctrl_timer_view_tick(void* p){
        view_frame_tick(view_default());
}

void _ctrl_timer_read_dht(void* p){
        dht_data_t* dht_data = &(view_default()->view_data.dht_data);
        result_t ret = dht_read(dht_default(), dht_data);
        if(ret != ERR_OK) {
                printf("dht read error:%d\n", ret);
                return;
        }
        //printf("dht: %d %d\n", dht_data->humidity, dht_data->temperature);
}

void _ctrl_switch_mode(ctrl_t* ctrl, uint8_t ctrl_mode){
        view_t* view = view_default();
        view_data_t* view_data = &(view->view_data);
        if(ctrl_mode != view_data->ctrl_mode) {
                view_data->ctrl_mode = ctrl_mode;
                if(ctrl_mode == CM_NORMAL) {
                        //TODO save setting
                        view_show_chain(view, _view_chain_default);
                }else{
                        view_show_page(view, PG_DATE_SET);
                }
        }

}

////////////////////////////////////////////////////////////////////////////////

void _ctrl_callback_on_key(uint8_t key, uint8_t event){
        view_t* view = view_default();
        ctrl_t* ctrl = ctrl_default();
        view_data_t * view_data = &(view_default()->view_data);
        if(key == KEY_ADC_KEY_2_VALUE) {
                if( event == KEY_EVENT_KEYDOWN_LONG) {
                        //enter setting mode
                        if(view_data->ctrl_mode == CM_NORMAL) {
                                _ctrl_switch_mode(ctrl, CM_SETTING);
                        }
                }else if(event == KEY_EVENT_KEYDOWN) {
                        if(view_data->ctrl_mode == CM_SETTING) {
                                //next set field
                                if(view_data->set_field == SF_YEAR) {
                                        //exit set mode
                                        _ctrl_switch_mode(ctrl, CM_NORMAL);
                                }else{
                                        //switch to next field
                                        view_data->set_field++;
                                        if(view_data->set_field == SF_MONTH) {
                                                view_show_page(view, PG_DATE_SET);
                                        }else if(view_data->set_field == SF_YEAR) {
                                                view_show_page(view, PG_YEAR_SET);
                                        }else if(view_data->set_field == SF_FONT) {
                                                view_show_page(view, PG_FONT_SET);
                                        }
                                }
                        }
                }

        }


        if(key == KEY_1 && event == KEY_EVENT_KEYDOWN) {
                if(view_data->ctrl_mode == CM_SETTING) {
                        _ctrl_adjust_field_value(ctrl_default(), INCR);
                }
        }

        if(key == KEY_3 && event == KEY_EVENT_KEYDOWN) {
                if(view_data->ctrl_mode == CM_SETTING) {
                        _ctrl_adjust_field_value(ctrl_default(), DECR);
                }
        }
}

void _ctrl_callback_on_lux(uint16_t lux){
        printf("lux: %d\n",lux);
        fb_t* fb = fb_default();
        if(lux < 100) {
                fb_set_brightness(fb, 7);
        }else if(lux < 150) {
                fb_set_brightness(fb, 6);
        }else if(lux < 200) {
                fb_set_brightness(fb, 5);
        }else if(lux < 250) {
                fb_set_brightness(fb, 4);
        }else if(lux < 300) {
                fb_set_brightness(fb, 3);
        }else if(lux < 350) {
                fb_set_brightness(fb, 2);
        }else if(lux < 400) {
                fb_set_brightness(fb, 1);
        }else{
                fb_set_brightness(fb, 0);
        }
}

// magic, haha
#define ADD_DELTA(ob, f, min, max, delta) {if(ob->f + delta == max) {ob->f = min;}else if(ob->f + delta == min) {ob->f = max;}else{ ob->f++;}}

void _ctrl_adjust_field_value(ctrl_t* ctrl, uint8_t delta){
        view_data_t* view_data = &(view_default()->view_data);
        rtc_date_time_t* dt = &(view_data->rtc_date_time);
        switch(view_data->set_field) {
        case SF_MINUTE: ADD_DELTA(dt, minute, 0, 60, delta); break;
        case SF_HOUR: ADD_DELTA(dt, hour, 0, 24, delta); break;
        case SF_DAY: ADD_DELTA(dt, day, 1, 31, delta); break;
        case SF_MONTH: ADD_DELTA(dt, month, 1, 12, delta); break;
        case SF_YEAR: ADD_DELTA(dt, year, 2000, 2099, delta); break;
        }
}
