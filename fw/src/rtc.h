#ifndef RTC_H
#define RTC_H

#include "base.h"
#include "i2c.h"

#define RTC_ADDR 0x68

// rtc struct
typedef struct _rtc_ {
        uint8_t rtc_addr;
        i2c_t* i2c;
} rtc_t;


// data read for ds3231n
typedef struct _rtc_data_ {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        bool_t h24;
        bool_t am;
        uint8_t day_of_week;
        uint16_t temperature;
}rtc_data_t;

////////////////////////////////////////////////////////////////////////////////
// public functions
////////////////////////////////////////////////////////////////////////////////
rtc_t* rtc_default();
void rtc_init(rtc_t* rtc);
result_t rtc_read(rtc_t* rtc, rtc_data_t* rtc_data);
result_t rtc_write(rtc_t* rtc, rtc_data_t* rtc_data);


///////////////////////////////////////////////////////////////////////////////
// private functions
//////////////////////////////////////////////////////////////////////////////
byte_t _rtc_bcd2dec(byte_t b);
byte_t _rtc_dec2bcd(byte_t b);


#endif
