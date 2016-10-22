#ifndef DHT_H
#define DHT_H

#include "base.h"

#define ERR_TIMEOUT 2
#define ERR_CRC_ERR 3

#define DHT_DATA_PIN 2

#define DHT_WAIT_CYCLE 10000

typedef struct _dht_{
    uint8_t data_pin;
} dht_t;

typedef struct _dht_data_{
    uint16_t temperature;
    uint16_t humidity;
} dht_data_t;


/////////////////////////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////////////////////////
dht_t* dht_default();
void dht_init(dht_t* dht);
result_t dht_read(dht_t* dht, dht_data_t* dht_data);


/////////////////////////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////////////////////////
bool_t _dht_wait_input(dht_t* dht, uint8_t expected);

#endif
