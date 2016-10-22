#include "key.h"

key_t _key0;


key_t* key_default(){
        return &_key0;
}


void key_init(key_t* key, key_event_fn_t event_fn){
        key->adc_pin = A2;
        key->event_fn = event_fn;
}


void key_scan(key_t* key){
        uint16_t val = analogRead(key->adc_pin);
        uint8_t k = _key_decide_key(val);


        if(k == KEY_NONE){
            // No key pressed
            if(key->pressed_key == KEY_NONE){
              return;
            }
            //has key pressed
            key->released_cycle++;
            if( key->released_cycle > KEY_MAX_RELEASED_CYCLE ){
                _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                _key_reset_key(key);
                return;
            }
        }

        // new key pressed
        if(key->pressed_key == KEY_NONE){
            key->pressed_key = k;
            key->pressed_cycle = 1;
            return;
        }

        // else pressed key
        if(key->pressed_key != k){
          //key conflict??
            _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
            _key_reset_key(key);
            return;
        }

        //key press hold
        key->pressed_cycle++;
        if(key->pressed_cycle >= KEY_MAX_PRESSED_CYCLE){

          key->trigger_cycle++;
          if(key->trigger_cycle == KEY_MAX_TRIGGER_CYCLE){
            key->trigger_cycle = 0;
          }

          if(key->trigger_cycle == 0){
            _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYDOWN);
            return;
          }
        }
}

void _key_reset_key(key_t* key){
  key->released_cycle = 0;
  key->pressed_key = KEY_NONE;
  key->pressed_cycle = 0;
  key->trigger_cycle = 0;
}


void _key_trigger_event(key_t* key, uint8_t k, uint8_t event){
    if(key->event_fn != NULL){
      key->event_fn(k, event);
    }
}

uint8_t _key_decide_key(uint16_t val){
        if(val == KEY_ADC_KEY_NONE_VALUE) {
                return KEY_NONE;
        }

        if( val >= KEY_ADC_KEY_1_VALUE - KEY_ADC_KEY_THRESHOLD
            && val <= KEY_ADC_KEY_1_VALUE + KEY_ADC_KEY_THRESHOLD) {
                return KEY_1;
        }

        if(val >= KEY_ADC_KEY_2_VALUE - KEY_ADC_KEY_THRESHOLD
           && val <= KEY_ADC_KEY_2_VALUE + KEY_ADC_KEY_THRESHOLD) {
                return KEY_2;
        }

        if(val >= KEY_ADC_KEY_3_VALUE - KEY_ADC_KEY_THRESHOLD
           && val <= KEY_ADC_KEY_3_VALUE + KEY_ADC_KEY_THRESHOLD) {
                return KEY_3;
        }

        return KEY_NONE;
}
