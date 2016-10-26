#include "setting.h"
#include <avr/eeprom.h>

setting_data_t _setting_data EEMEM;

result_t setting_read(setting_data_t* data){
        eeprom_busy_wait();
        eeprom_read_block(&_setting_data, data, sizeof(setting_data_t));
        return ERR_OK;
}


result_t setting_write(setting_data_t* data){
        eeprom_busy_wait();
        eeprom_write_block(&_setting_data, data, sizeof(setting_data_t));
        printf("setting written!!%d\n", data->font_type);
        return ERR_OK;
}
