#ifndef LUX_H
#define LUX_H
#include "base.h"

#define LUX_SAMPLE_SIZE 8
#define LUX_PIN A3

typedef void (*lux_callback_fn_t)(uint16_t lux);

typedef struct _lux_{
    uint8_t lux_pin;
    uint16_t lux_recent;
    lux_callback_fn_t callback_fn;
    uint16_t sample_data[LUX_SAMPLE_SIZE];
    uint8_t  sample_points;
}lux_t;

///////////////////////////////////////////////////////
// public functions
///////////////////////////////////////////////////////
lux_t* lux_default();
void lux_init(lux_t* lux, lux_callback_fn_t callback_fn);
void lux_sample(lux_t* lux);
uint16_t lux_recent(lux_t* lux);


///////////////////////////////////////////////////////
// private functions
///////////////////////////////////////////////////////
void _lux_calc_recent(lux_t* lux);


#endif
