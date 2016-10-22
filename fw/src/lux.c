#include "lux.h"
lux_t _lux0;

lux_t* lux_default(){
        return &_lux0;
}

void lux_init(lux_t* lux, lux_callback_fn_t callback_fn){
        lux->lux_pin = LUX_PIN;
        lux->callback_fn = callback_fn;
        lux->lux_recent = 0;
        lux->sample_points = 0;
        zero(lux->sample_data, sizeof(lux->sample_data));
}

void lux_sample(lux_t* lux){
        lux->sample_data[lux->sample_points++] = analogRead(lux->lux_pin);
        if(lux->sample_points == LUX_SAMPLE_SIZE) {
                _lux_calc_recent(lux);
        }
}


uint16_t lux_recent(lux_t* lux){
        return lux->lux_recent;
}


void _lux_calc_recent(lux_t* lux){
        uint32_t total = 0;
        for(uint8_t i=0; i< LUX_SAMPLE_SIZE; i++) {
                total += lux->sample_data[i];
        }
        lux->lux_recent = total / LUX_SAMPLE_SIZE;
        lux->sample_points = 0;

        if(lux->callback_fn != NULL) {
                lux->callback_fn(lux->lux_recent);
        }
}
