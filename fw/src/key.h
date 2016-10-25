#ifndef KEY_H
#define KEY_H

#include "base.h"

#define KEY_NONE 0
#define KEY_1 1
#define KEY_2 2
#define KEY_3 3

#define KEY_EVENT_KEYDOWN 1
#define KEY_EVENT_KEYUP   2
#define KEY_EVENT_KEYDOWN_LONG 3
//#define KEY_EVENT_KEYPRESS   3
//KEY_DOWN -> KEY_UP ->KEYPRESS

#define KEY_SCAN_PEROD_MS portTICK_PERIOD_MS
#define KEY_SAMPLE_POINTS   32
#define KEY_SAMPLE_DATA_SIZE KEY_SAMPLE_POINTS / 4 //2 bits a point

#define KEY_ADC_KEY_NONE_VALUE 0
#define KEY_ADC_KEY_1_VALUE 512
#define KEY_ADC_KEY_2_VALUE 340
#define KEY_ADC_KEY_3_VALUE 255
#define KEY_ADC_KEY_THRESHOLD 32

#define KEY_MAX_RELEASED_CYCLE 5
#define KEY_MAX_PRESSED_CYCLE 2
#define KEY_MAX_TRIGGER_CYCLE 10
#define KEY_MAX_PRESSED_LONG_CYCLE 10 * 15


//callback defines
typedef void (*key_event_fn_t)(uint8_t key, uint8_t event);

typedef struct _key_ {
  uint8_t adc_pin;
  bool_t pressed_long;
  uint8_t pressed_key;
  uint8_t pressed_cycle;
  uint8_t trigger_cycle;
  uint8_t released_cycle;
  key_event_fn_t event_fn;
} key_t;



////////////////////////////////////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////////////////////////////////////
key_t* key_default();
void key_init(key_t* key, key_event_fn_t event_fn);
void key_scan(key_t* key);    //should call in 15ms interval


////////////////////////////////////////////////////////////////////////////////
// private functions
////////////////////////////////////////////////////////////////////////////////
uint8_t _key_decide_key(uint16_t val);
void _key_reset_key(key_t* key);
void _key_trigger_event(key_t* key, uint8_t k, uint8_t event);

#endif
