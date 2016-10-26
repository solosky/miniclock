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

#define KEY_MAX_RELEASING_TICK 5
#define KEY_MAX_PRESSING_TICK 2
#define KEY_MAX_PRESSED_LONG_TICK 10 * 10

enum _key_state_{
    KS_NONE, //No key pressed
    KS_PRESSING, //first press, waiting stable
    KS_PRESSED,  // pressed stably
    KS_PRESSED_LONG, // long pressed
    KS_RELEASING,  //first release, wating stable
};


//callback defines
typedef void (*key_event_fn_t)(uint8_t key, uint8_t event);

typedef struct _key_ {
  uint8_t adc_pin;
  uint8_t key_state;
  uint8_t pressed_key;
  uint8_t pressing_tick;
  uint8_t pressed_tick;
  uint8_t releasing_tick;
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
