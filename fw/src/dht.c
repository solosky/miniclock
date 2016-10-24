#include "dht.h"

dht_t _dht0;

dht_t* dht_default(){
        return &_dht0;
}


void dht_init(dht_t* dht){
        dht->data_pin = DHT_DATA_PIN;
}


result_t dht_read(dht_t* dht, dht_data_t* dht_data){
        // REQUEST SAMPLE
        pinMode(dht->data_pin, OUTPUT);
        digitalWrite(dht->data_pin, LOW);
        delay(18);
        digitalWrite(dht->data_pin, HIGH);
        delayMicroseconds(40);
        pinMode(dht->data_pin, INPUT);

        // wait dht pull low
        if(!_dht_wait_input(dht, LOW)) {
                return ERR_TIMEOUT;
        }

        // wait dht ready pluse
        if(!_dht_wait_input(dht, HIGH)) {
                return ERR_TIMEOUT;
        }
        if(!_dht_wait_input(dht, LOW)) {
                return ERR_TIMEOUT;
        }

        //here comes data 40 bit
        zero(dht_data, sizeof(dht_data_t));
        // BUFFER TO RECEIVE
        uint8_t bits[5];
        uint8_t cnt = 7;
        uint8_t idx = 0;

        zero(bits, sizeof(bits));

        // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
        for (int i=0; i<40; i++) {
                if(!_dht_wait_input(dht, HIGH)) {
                        return ERR_TIMEOUT;
                }

                unsigned long t = micros();

                if(!_dht_wait_input(dht, LOW)) {
                        return ERR_TIMEOUT;
                }

                if ((micros() - t) > 40) {
                        bits[idx] |= (1 << cnt);
                }

                if (cnt == 0) {
                        cnt = 7; // restart at MSB
                        idx++; // next byte!
                }else {
                        cnt--;
                }
        }

        uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];
        if (bits[4] != sum) {
                return ERR_CRC_ERR;
        }

        //printf("%d,%d,%d,%d,%d\n", bits[0], bits[1], bits[2], bits[3], bits[4]);

        // dht 11
        if(bits[1] == 0 && bits[3] == 0) {
                dht_data->humidity = bits[0] * 10;
                dht_data->temperature = bits[2] * 10;
        }else{ // dht 22

                dht_data->humidity    = bits[0] << 8;
                dht_data->humidity   += bits[1];
                dht_data->temperature = bits[2] << 8;
                dht_data->temperature += bits[3];
        }


        return ERR_OK;
}


bool_t _dht_wait_input(dht_t* dht, uint8_t expected){
        uint16_t loop = DHT_WAIT_CYCLE;
        while(digitalRead(dht->data_pin) != expected) {
                if(loop-- == 0) {
                        return false;
                }
        }
        return true;
}
