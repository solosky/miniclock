#ifndef CTRL_H
#define CTRL_H

#include "base.h"
#include "dht.h"
#include "lux.h"
#include "key.h"
#include "i2c.h"
#include "rtc.h"
#include "fb.h"
#include "usart.h"


typedef struct _ctrl_{
    dht_data_t dht_data;
    rtc_date_time_t rtc_date_time;
    uint8_t state;

} ctrl_t;

typedef void (*timer_fn_t)(void*);



///////////////////////////////////////////////////////////////////
// public functions
///////////////////////////////////////////////////////////////////
ctrl_t* ctrl_default();
void ctrl_init(ctrl_t* ctrl);

///////////////////////////////////////////////////////////////////
// private functions
///////////////////////////////////////////////////////////////////
// global callbacks
void _ctrl_callback_on_key(uint8_t key, uint8_t event);
void _ctrl_callback_on_lux(uint16_t lux);


// main timer callbacks
void _ctrl_timer_key_scan(void* p);
void _ctrl_timer_read_lux(void* p);
void _ctrl_timer_read_dht(void* p);
void _ctrl_timer_flash_dot(void* p);


// helper functions
void _ctrl_init_timers(ctrl_t* ctrl);
void _ctrl_new_timer(const char* name, uint16_t tick_count, timer_fn_t timer_fn);




#endif
