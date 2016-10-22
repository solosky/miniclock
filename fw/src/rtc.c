#include "rtc.h"
#include "i2c.h"
#include "debug.h"

rtc_t _rtc0;





rtc_t* rtc_default(){
        return &_rtc0;
}

void rtc_init(rtc_t* rtc){
        rtc->rtc_addr = RTC_ADDR;
        rtc->i2c = i2c_default();
        i2c_init(rtc->i2c, 0);

        //1Hz pin enable pull up TODO ..
        pinMode(RTC_INT_PIN, INPUT_PULLUP);
        EICRA = _BV(ISC11) | _BV(ISC10);
        EIMSK = _BV(INT1);


        //init control register
        _rtc_write_regiter(rtc, 0xE, 0);  // 1Hz SqWave, disable all alarms
        _rtc_write_regiter(rtc, 0xF, 0);  // disable 32KHz output

        rtc_date_time_t rtc_date_time;
        rtc_read(rtc, &rtc_date_time);
        _rtc_sync_rtc_time(rtc, &rtc_date_time);
}

result_t rtc_read(rtc_t* rtc, rtc_date_time_t* rtc_date_time){
        result_t ret;
        byte_t buff[7];

        ret = _rtc_read_regiter(rtc, 0, buff, sizeof(buff));
        if(ret != ERR_OK) {
                return ret;
        }

        // parse rtc info
        rtc_date_time->second = _rtc_bcd2dec(buff[0]);   //second
        rtc_date_time->minute = _rtc_bcd2dec(buff[1]);   //minute
        rtc_date_time->h24    = (buff[2] >> 6) == 0;
        if(rtc_date_time->h24) {
                rtc_date_time->hour = _rtc_bcd2dec(buff[2] & 0x3F);
        }else{
                rtc_date_time->pm   = (buff[2] & _BV(5)) > 0;
                rtc_date_time->hour = _rtc_bcd2dec(buff[2] & 0x1F); //hour
        }
        rtc_date_time->week = _rtc_bcd2dec(buff[3]);
        rtc_date_time->day = _rtc_bcd2dec(buff[4]);
        rtc_date_time->month = _rtc_bcd2dec(buff[5] & 0x1F);
        rtc_date_time->year = _rtc_bcd2dec(buff[6]);

        return ERR_OK;
}


result_t rtc_write(rtc_t* rtc, rtc_date_time_t* rtc_date_time){
        result_t ret;
        byte_t buff[8];

        buff[0] = 0;  // first reg address bytes

        buff[1] = _rtc_dec2bcd(rtc_date_time->second);
        buff[2] = _rtc_dec2bcd(rtc_date_time->minute);
        if(rtc_date_time->h24) {
                buff[3] = _rtc_bcd2dec(rtc_date_time->hour) | _BV(6);
        }else{
                buff[3] = _rtc_bcd2dec(rtc_date_time->hour);
                if(rtc_date_time->pm) {
                        buff[3] |= _BV(5);
                }
        }
        buff[4] = _rtc_dec2bcd(rtc_date_time->week);
        buff[5] = _rtc_dec2bcd(rtc_date_time->day);
        buff[6] = _rtc_dec2bcd(rtc_date_time->month);
        buff[7] = _rtc_dec2bcd(rtc_date_time->year);

        ret = i2c_write_buff(rtc->i2c, rtc->rtc_addr, buff, sizeof(buff));
        if(ret != ERR_OK) {
                printf("rtc_write data error: %d\n", ret);
                return ret;
        }

        return ERR_OK;
}


///////////////////////////////////////////////////////////////////////////////
// private functions
//////////////////////////////////////////////////////////////////////////////

byte_t _rtc_bcd2dec(byte_t val){
        return ( (val/16*10) + (val%16) );
}


byte_t _rtc_dec2bcd(byte_t val){
        return ( (val/10*16) + (val%10) );
}

void _rtc_sync_rtc_time(rtc_t* rtc, rtc_date_time_t* rtc_date_time){
        rtc_time_t* tm = &(rtc->rtc_time);
        tm->hour = rtc_date_time->hour;
        tm->second = rtc_date_time->second;
        tm->minute = rtc_date_time->minute;
        if(!rtc_date_time->h24 && rtc_date_time->pm) {
                tm->hour += 12;
        }
}


result_t _rtc_read_regiter(rtc_t* rtc, byte_t reg, byte_t* buff, uint8_t size){
        // write address
        result_t ret;
        ret = i2c_write(rtc->i2c, rtc->rtc_addr, reg);
        if(ret != ERR_OK) {
                printf("rtc_read: write reg addr error: %d\n", ret);
                return ret;
        }
        ret = i2c_read_buff(rtc->i2c, rtc->rtc_addr, buff, 7);
        if(ret != ERR_OK) {
                printf("rtc_read: read data error: %d\n", ret);
                return ret;
        }
        return ret;
}


result_t _rtc_write_regiter(rtc_t* rtc, byte_t reg, byte_t value){
        // write address
        result_t ret;
        byte_t buff[2];

        buff[0] = reg;
        buff[1] = value;

        ret = i2c_write_buff(rtc->i2c, rtc->rtc_addr, buff, sizeof(buff));
        if(ret != ERR_OK) {
                printf("rtc_read: write reg addr error: %d\n", ret);
                return ret;
        }
        return ERR_OK;
}

ISR(INT1_vect){
        rtc_time_t* tm = &(_rtc0.rtc_time);
        if(tm->second < 59) {
                tm->second++;
                return;
        }

        tm->second = 0;
        if(tm->minute < 59) {
                tm->minute++;
                return;
        }
        tm->minute = 0;
        if(tm->hour < 23) {
                tm->hour++;
                return;
        }
        tm->hour = 0;
}
