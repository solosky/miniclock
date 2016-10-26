#ifndef SETTING_H
#define SETTING_H

#include "base.h"


//setting info, store in eeprom
typedef struct _setting_data_ {
        uint8_t font_type;
} setting_data_t;


///////////////////////////////////////////////////////////////////////////////
// public functions
///////////////////////////////////////////////////////////////////////////////
result_t setting_read(setting_data_t* data);
result_t setting_write(setting_data_t* data);

#endif
