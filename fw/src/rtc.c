#include "rtc.h"
#include "i2c.h"
#include "debug.h"

rtc_t _rtc0;


byte_t _rtc_bcd2dec(byte_t val){
        return ( (val/16*10) + (val%16) );
}
byte_t _rtc_dec2bcd(byte_t val){
        return ( (val/10*16) + (val%10) );
}




rtc_t* rtc_default(){
        return &_rtc0;
}

void rtc_init(rtc_t* rtc){
        rtc->rtc_addr = RTC_ADDR;
        rtc->i2c = i2c_default();
        i2c_init(rtc->i2c, 0);
}

result_t rtc_read(rtc_t* rtc, rtc_data_t* rtc_data){
        result_t ret;
        byte_t buff[7];

        // write address
        ret = i2c_write(rtc->i2c, rtc->rtc_addr, 0);
        if(ret != ERR_OK) {
                printf("write_error: %d\n", ret);
                return ret;
        }
        ret = i2c_read_buff(rtc->i2c, rtc->rtc_addr, buff, 7);
        if(ret != ERR_OK) {
                printf("read error: %d\n", ret);
                return ret;
        }

        // parse rtc info
        rtc_data->second = _rtc_bcd2dec(buff[0]);   //second
        rtc_data->minute = _rtc_bcd2dec(buff[1]);   //minute
        rtc_data->h24    = (buff[2] >> 6) == 0;
        if(rtc_data->h24) {
                rtc_data->hour = _rtc_bcd2dec(buff[2] & 0x3F);
        }else{
                rtc_data->am   = (buff[2] & _BV(5)) > 0;
                rtc_data->hour = _rtc_bcd2dec(buff[2] & 0x1F); //hour
        }
        rtc_data->day_of_week = _rtc_bcd2dec(buff[3]);
        rtc_data->day = _rtc_bcd2dec(buff[4]);
        rtc_data->month = _rtc_bcd2dec(buff[5] & 0x1F);
        rtc_data->year = _rtc_bcd2dec(buff[6]);

        return ERR_OK;
}


result_t rtc_write(rtc_t* rtc, rtc_data_t* rtc_data){
        return ERR_OK;
}
