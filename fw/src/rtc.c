#include "rtc.h"

rtc_t _rtc0;


rtc_t* rtc_default(){
  return &_rtc0;
}

void rtc_init(rtc_t* rtc){
  
}

result_t rtc_read(rtc_t* rtc, rtc_data_t* rtc_data){
    return ERR_OK;
}


result_t rtc_write(rtc_t* rtc, rtc_data_t* rtc_data){
  return ERR_OK;
}
