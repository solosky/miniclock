#ifndef VIEW_H
#define VIEW_H

#include "base.h"
#include "dht.h"
#include "rtc.h"


enum _view_id {
    VI_TIME,  //hour:minute
    VI_DATE,  //month day
    VI_YEAR,  //year
    VI_TEMP,  //temperature
    VI_HUMIDITY, //humidity
    VI_LUX,  //LUX, just for test
    VI_DEBUG  //hello world~
};

//view data, shared with ctrl module
typedef struct _view_data_{
    dht_data_t dht_data;
    rtc_date_time_t rtc_date_time;
}view_data_t;

//render callbacks to render chars on led
struct _view_page_;
typedef void (*page_render_fn_t)(struct _view_page_* vp, struct _view_data_* vd);

//single view page
typedef struct _view_page_{
  uint8_t view_id;
  uint16_t render_interval;
  page_render_fn_t render_fn;
} view_page_t;

//render chain define, used to auto flipping view page
typedef struct _view_chain_{
    uint8_t view_id;
    uint8_t next_view_id;
    bool_t should_scroll; //used to scroll context
    uint8_t scroll_delay; //
    uint8_t next_view_delay;
    uint8_t next_view_animation;  // TODO ..switch animation
} view_chain_t;



#endif
