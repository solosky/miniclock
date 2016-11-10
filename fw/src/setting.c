#include "setting.h"
#include <avr/eeprom.h>

setting_data_t _setting_data EEMEM;

result_t setting_read(setting_data_t* data){
        return _setting_read_buff((uint8_t*)&_setting_data, (byte_t*) data, sizeof(setting_data_t));
}


result_t setting_write(setting_data_t* data){
        return _setting_write_buff((uint8_t*)&_setting_data,  (byte_t*) data, sizeof(setting_data_t));
}


result_t _setting_write_buff(uint8_t* base, byte_t* buff, uint8_t size){
        cli(); // disable interrupt
        for(uint8_t i=0; i<size; i++) {
                eeprom_busy_wait();
                eeprom_write_byte(base + i, buff[i]);
        }
        sei(); // enable interrupt
        return ERR_OK;
}


result_t _setting_read_buff(uint8_t* base, byte_t* buff, uint8_t size){
        cli(); // disable interrupt
        for(uint8_t i=0; i<size; i++) {
                eeprom_busy_wait();
                buff[i] = eeprom_read_byte(base + i);
        }
        sei(); // enable interrupt
        return ERR_OK;
}
