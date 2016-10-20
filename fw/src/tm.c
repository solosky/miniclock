#include "tm.h"

void _tm_start(tm_t* tm) {
        digitalWrite(tm->din_pin, LOW);
        delayMicroseconds(5);
        digitalWrite(tm->sclk_pin, LOW);
}


void _tm_stop(tm_t* tm) {
        digitalWrite(tm->din_pin, LOW);
        delayMicroseconds(5);
        digitalWrite(tm->sclk_pin, HIGH);
        delayMicroseconds(5);
        digitalWrite(tm->din_pin, HIGH);
}


void _tm_shift_out(tm_t* tm, uint8_t data){
        for (uint8_t i = 0; i < 8; i++) {
                digitalWrite(tm->din_pin, (data >> i) & 1);
                digitalWrite(tm->sclk_pin, HIGH);
                delayMicroseconds(5);
                digitalWrite(tm->sclk_pin, LOW);
                delayMicroseconds(5);
        }
}


void _tm_cmd(tm_t* tm, uint8_t cmd) {
        _tm_start(tm);
        _tm_shift_out(tm, cmd);
        _tm_stop(tm);
}


void _tm_write(tm_t* tm, uint8_t cmd, uint8_t data){
        _tm_start(tm);
        _tm_shift_out(tm, cmd);
        _tm_shift_out(tm, data);
        _tm_stop(tm);
}


void _tm_write_buff(tm_t* tm, uint8_t cmd, uint8_t* buff, uint8_t len){
        _tm_start(tm);
        _tm_shift_out(tm, cmd);
        for (uint8_t i = 0; i < len; i++) {
                _tm_shift_out(tm, buff[i]);
        }
        _tm_stop(tm);
}


void tm_init(tm_t* tm, uint8_t sclk_pin, uint8_t din_pin){
        tm->sclk_pin = sclk_pin;
        tm->din_pin = din_pin;

        pinMode(sclk_pin, OUTPUT);
        pinMode(din_pin, OUTPUT);

        digitalWrite(sclk_pin, HIGH);
        digitalWrite(din_pin, HIGH);
}

void tm_set_brightless(tm_t* tm, uint8_t level){
        _tm_cmd(tm, TM_CMD_BRIGHNESS | (level & 0x8));
}


void tm_display(tm_t* tm, uint8_t* mem, uint8_t size){
      _tm_cmd(tm, TM_CMD_ADDR_INC);
      _tm_write_buff(tm, TM_ADDR_BASE, mem, size);
}
