#ifndef RTC_H
#define RTC_H

#include "base.h"
#include "i2c.h"

#define RTC_ADDR 0x68
#define RTC_INT_PIN 3

typedef struct _rtc_time_{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} rtc_time_t;

// rtc struct
typedef struct _rtc_ {
        uint8_t rtc_addr;
        i2c_t* i2c;
        rtc_time_t rtc_time;  //simple time, driving by 1hz
} rtc_t;



// data read for ds3231n
typedef struct _rtc_date_time_ {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        bool_t h24;
        bool_t pm;
        uint8_t week;
        uint16_t temperature;
}rtc_date_time_t;

////////////////////////////////////////////////////////////////////////////////
// public functions
////////////////////////////////////////////////////////////////////////////////
rtc_t* rtc_default();
void rtc_init(rtc_t* rtc, i2c_t* i2c);
result_t rtc_read(rtc_t* rtc, rtc_date_time_t* rtc_date_time);
result_t rtc_write(rtc_t* rtc, rtc_date_time_t* rtc_date_time);


///////////////////////////////////////////////////////////////////////////////
// private functions
//////////////////////////////////////////////////////////////////////////////
byte_t _rtc_bcd2dec(byte_t b);
byte_t _rtc_dec2bcd(byte_t b);
result_t _rtc_read_regiter(rtc_t* rtc, byte_t reg, byte_t* buff, uint8_t size);
result_t _rtc_write_regiter(rtc_t* rtc, byte_t reg, byte_t value);
void _rtc_sync_rtc_time(rtc_t* rtc, rtc_date_time_t* rtc_date_time);


#endif
