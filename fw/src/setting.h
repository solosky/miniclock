#ifndef SETTING_H
#define SETTING_H

#include "base.h"

#define SETTING_MAX_ALARM_SIZE 2

typedef struct _alarm_data_{
    uint8_t hour;
    uint8_t minute;
    uint8_t repeat; // obit represent a day
} alarm_data_t;

//setting info, store in eeprom
typedef struct _setting_data_ {
        uint8_t font_type;
        int8_t lux_adj;
        alarm_data_t alarms[SETTING_MAX_ALARM_SIZE];  //TODO need implements
} setting_data_t;


///////////////////////////////////////////////////////////////////////////////
// public functions
///////////////////////////////////////////////////////////////////////////////
result_t setting_read(setting_data_t* data);
result_t setting_write(setting_data_t* data);


///////////////////////////////////////////////////////////////////////////////
// private functions
///////////////////////////////////////////////////////////////////////////////
result_t _setting_write_buff(uint8_t* base, byte_t* buff, uint8_t size);
result_t _setting_read_buff(uint8_t* base, byte_t* buff, uint8_t size);


#endif
