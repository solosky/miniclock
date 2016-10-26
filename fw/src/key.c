#include "key.h"

key_t _key0;


key_t* key_default(){
        return &_key0;
}


void key_init(key_t* key, key_event_fn_t event_fn){
        zero(key, sizeof(key_t));
        key->adc_pin = A2;
        key->event_fn = event_fn;
}


void key_scan(key_t* key){
        uint16_t val = analogRead(key->adc_pin);
        uint8_t k = _key_decide_key(val);
        if(key == KEY_NONE) {
                switch(key->key_state) {
                case KS_NONE: break;
                case KS_PRESSING: {
                        _key_reset_key(key);
                } break;
                case KS_RELEASING: {
                        if(key->releasing_tick++ > KEY_MAX_RELEASING_TICK) {
                                _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                                _key_reset_key(key);
                        }
                } break;

                case KS_PRESSED:
                case KS_PRESSED_LONG: {
                        key->key_state = KS_RELEASING;
                } break;
                }
        }else{
                switch(key->key_state) {
                case KS_NONE: {
                        key->key_state = KS_PRESSING;
                        key->pressed_key = k;
                } break;
                case KS_PRESSING: {
                        // conflict check
                        if(k != key->pressed_key) {
                                key->pressed_tick = 0;
                                return;
                        }
                        if(key->pressing_tick++ > KEY_MAX_PRESSING_TICK) {
                                key->key_state = KS_PRESSED;
                                _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYDOWN);
                        }
                } break;
                case KS_PRESSED: {
                        // key conflict check
                        if(k != key->pressed_key) {
                                _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                                _key_reset_key(key);
                                return;
                        }
                        //long press check
                        if(key->pressed_tick++ > KEY_MAX_PRESSED_LONG_TICK) {
                                key->key_state = KS_PRESSED_LONG;
                                _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYDOWN_LONG);

                        }
                } break;
                case KS_PRESSED_LONG: {
                        // key conflict check
                        if(k != key->pressed_key) {
                                _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                                _key_reset_key(key);
                        }
                } break;
                }
        }


        switch(key->key_state) {
        case KS_NONE: {
                // no key pressed
                if(k == KEY_NONE) {
                        return;
                }
                // init key pressed
                key->key_state = KS_PRESSING;
                key->pressed_key = k;
                key->pressing_tick = 1;
        }
        break;

        case KS_PRESSING: {
                if(k == KEY_NONE) {
                        _key_reset_key(key);
                        return;
                }
                //key conflict??
                if(k != key->pressed_key) {
                        _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                        _key_reset_key(key);
                        return;
                }

                // continue pressed
                if(key->pressing_tick++ > KEY_MAX_PRESSING_TICK) {
                        key->key_state = KS_PRESSED;
                        _key_trigger_event(key, k, KEY_EVENT_KEYDOWN);
                        return;
                }
        }; break;

        case KS_PRESSED: {
                //release check
                if(key == KEY_NONE) {
                        _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                        _key_reset_key(key);
                        return;
                }

                // conflict check
                if(k != key->pressed_key) {
                        _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                        _key_reset_key(key);
                        return;
                }


        }; break;

        case KS_PRESSED_LONG: {
                //release check
                if(key == KEY_NONE) {
                        _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                        _key_reset_key(key);
                        return;
                }

                //conflict check
                if(k != key->pressed_key) {
                        _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                        _key_reset_key(key);
                        return;
                }

        }; break;

        case KS_RELEASING: {
                if(key == KEY_NONE && key->releasing_tick++ > KEY_MAX_RELEASING_TICK) {
                        _key_trigger_event(key, key->pressed_key, KEY_EVENT_KEYUP);
                        _key_reset_key(key);
                        return;
                }
        }
        }
}

void _key_reset_key(key_t* key){
        key->pressing_tick = 0;
        key->releasing_tick = 0;
        key->pressed_tick = 0;
        key->key_state = KS_NONE;
        key->pressed_key = KEY_NONE;
}



void _key_trigger_event(key_t* key, uint8_t k, uint8_t event){
        if(key->event_fn != NULL) {
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
