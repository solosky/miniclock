#ifndef VIEW_H
#define VIEW_H

#include "base.h"
#include "dht.h"
#include "rtc.h"
#include "timer.h"
#include "fb.h"
#include <stdlib.h>
#include <stdio.h>

#define ERR_RECORD_NOT_FOUND 3

enum _page_id {
        PG_TIME, //hour:minute
        PG_DATE, //month day
        PG_YEAR, //year
        PG_TEMP, //temperature
        PG_HUMIDITY, //humidity
        PG_LUX, //LUX, just for test
        PG_DEBUG //hello world~
};

//view data, shared with ctrl module
typedef struct _view_data_ {
        uint8_t mode; //display mode, or setting mode
        uint8_t set_field; //whitch field to set
        dht_data_t dht_data;
        rtc_date_time_t rtc_date_time;
}view_data_t;

//render callbacks to render chars on led
struct _view_;
typedef void (*page_render_fn_t)(struct _view_ * vr);

//single view page
typedef struct _view_page_ {
        uint8_t page_id;
        uint16_t render_interval;
        page_render_fn_t render_fn;
} view_page_t;

//render chain define, used to auto flipping view page
typedef struct _view_chain_ {
        uint8_t page_id;
        uint8_t next_page_id;
        uint16_t next_page_delay;
        //uint8_t next_view_animation;  // TODO ..switch animation
        //bool_t should_scroll; //used to scroll context
        //uint8_t scroll_delay; //
} view_chain_t;


//global view info
typedef struct _view_ {
        fb_t* fb;           //frame buffer
        const view_page_t* view_page_head;
        const view_chain_t* view_chain_head; //
        view_data_t view_data; //shared with ctrl module

        //runtime info
        uint8_t render_tick; //current view tick,
        view_page_t view_page_current; //current view page,
        view_chain_t view_chain_current; //current view chain current
        timer_t render_tick_timer;
        timer_t next_page_timer;
} view_t;


extern const view_chain_t _view_chain_default[];

///////////////////////////////////////////////////////////////////////
// public functions
///////////////////////////////////////////////////////////////////////
view_t * view_default();
void view_init(view_t* view,fb_t* fb);
void view_show_page(view_t* view, uint8_t page_id);
void view_show_chain(view_t* view, const view_chain_t* head);


//////////////////////////////////////////////////////////////////////
// private functions
//////////////////////////////////////////////////////////////////////
void _view_render_time(view_t* view);
void _view_render_date(view_t* view);
void _view_render_temp(view_t* view);
void _view_render_humidity(view_t* view);

void _view_show_single_page(view_t* view, view_page_t* view_page);
void _view_stop_chain(view_t* view);
void _view_chain_next(view_t* view, bool_t first);

void _view_timer_next_page(void* p);
void _view_timer_render_page(void* p);

result_t _view_pgm_read_chain_first(view_t* view, view_chain_t* view_chain_out);
result_t _view_pgm_find_chain(view_t* view, uint8_t page_id, view_chain_t* view_chain_out);
result_t _view_pgm_find_page(view_t* view, uint8_t page_id, view_page_t* view_page_out);

#endif
