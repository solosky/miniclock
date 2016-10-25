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
#include "timer.h"
#include "view.h"

#define INCR 1
#define DECR -1


typedef struct _ctrl_ {
} ctrl_t;





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
void _ctrl_timer_view_tick(void* p);

void _ctrl_switch_mode(ctrl_t* ctrl, uint8_t ctrl_mode);


// helper functions
void _ctrl_init_timers(ctrl_t* ctrl);
void _ctrl_adjust_field_value(ctrl_t* ctrl, uint8_t delta);



#endif
