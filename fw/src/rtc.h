#ifndef RTC_H
#define RTC_H

#include "base.h"

// rtc struct
typedef struct _rtc_{
  uint8_t rtc_addr;
} rtc_t;


// data read for ds3231n
typedef struct _rtc_data_{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint16_t temperature;
}rtc_data_t;

////////////////////////////////////////////////////////////////////////////////
// public functions
////////////////////////////////////////////////////////////////////////////////
rtc_t* rtc_default();
void rtc_init(rtc_t* rtc);
result_t rtc_read(rtc_t* rtc, rtc_data_t* rtc_data);
result_t rtc_write(rtc_t* rtc, rtc_data_t* rtc_data);


#endif
