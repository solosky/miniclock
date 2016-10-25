#include "view.h"

view_t _view0;

//////////////////////////////////////////////////////////////////////
// static defines
//////////////////////////////////////////////////////////////////////
// view pages
//page id, render_interval, render fn
const view_page_t _view_pages[] PROGMEM = {
        {PG_TIME, VIEW_TICK_OF_TIME(500), _view_render_time},
        {PG_TEMP, VIEW_TICK_OF_TIME(1000), _view_render_temp},
        {PG_HUMIDITY, VIEW_TICK_OF_TIME(1000), _view_render_humidity},
        {PG_DATE, 0, _view_render_date},
        {PG_TIME_SET, VIEW_TICK_OF_TIME(1000), _view_render_time_set},
        {PG_DATE_SET, VIEW_TICK_OF_TIME(1000), _view_render_date_set},
        {PG_FONT_SET, VIEW_TICK_OF_TIME(1000), _view_render_font_set},
        {0,0,0}
};

//view chains
//page id, next page id, next delay
const view_chain_t _view_chain_default[] PROGMEM = {
        {PG_TIME, PG_DATE, VIEW_TICK_OF_TIME(5000)},
        {PG_DATE, PG_TEMP, VIEW_TICK_OF_TIME(3000)},
        {PG_TEMP, PG_HUMIDITY, VIEW_TICK_OF_TIME(3000)},
        {PG_HUMIDITY, PG_TIME, VIEW_TICK_OF_TIME(3000)},
        {0,0,0}
};


//////////////////////////////////////////////////////////////////////
// public function implements
/////////////////////////////////////////////////////////////////////
view_t * view_default(){
        return &_view0;
}


void view_init(view_t* view, fb_t* fb){
        view->fb = fb;
        view->view_page_head = _view_pages;
}

void view_frame_tick(view_t* view){
        //increment view tick
        view->render_tick++;

        //check if should re-render
        if(view->render_tick >= view->view_page_current.render_tick) {
                view->view_page_current.render_fn(view);
                view->render_tick = 0;
                view->render_cnt++;
        }

        //check if should flip to next page
        if(view->view_mode == VM_CHAIN) {
                view->next_page_tick++;
                if(view->next_page_tick >= view->view_chain_current.next_page_tick) {
                        _view_chain_next(view, false);
                }
        }
}


void view_show_page(view_t* view, uint8_t page_id){
        view_page_t page;
        result_t ret;

        //find page info
        ret = _view_pgm_find_page(view, page_id, &page);
        if(ret != ERR_OK) {
                printf("page not found: page=%d, code=%d\n", page_id, ret);
                return;
        }

        view->view_mode = VM_SINGLE;
        _view_show_single_page(view, &page);
}


void view_show_chain(view_t* view, const view_chain_t* head){
        view->view_chain_head = head;
        view->view_mode = VM_CHAIN;
        _view_chain_next(view, true);
}


/////////////////////////////////////////////////////////////////
// render functions
////////////////////////////////////////////////////////////////
void _view_render_time(view_t* view){
        char buff[6];
        //rtc_date_time_t* dt = &(view->view_data.rtc_date_time);
        rtc_time_t * tm = &(rtc_default()->rtc_time);
        char dot = (view->render_cnt & 1) ? ' ' : ':';
        sprintf(buff, "%02d%c%02d", tm->minute, dot,  tm->second);
        fb_clear_display(view->fb);
        fb_draw_string(view->fb, buff, 0, 0, font_4x7_led);
        fb_flush(view->fb);
}

void _view_render_date(view_t* view){
        char buff[6];
        rtc_date_time_t* dm = &(view->view_data.rtc_date_time);
        sprintf(buff, "%02d/%02d", dm->month, dm->day);
        fb_clear_display(view->fb);
        fb_draw_string(view->fb, buff, 0, 0, font_4x7_led);
        fb_flush(view->fb);
}

void _view_render_temp(view_t* view){
        char buff[6];
        dht_data_t* dht_data = &(view->view_data.dht_data);
        uint8_t temp_1 = dht_data->temperature / 10;
        uint8_t temp_2 = dht_data->temperature - temp_1 * 10;
        sprintf(buff, "%.02d.%.1d", temp_1, temp_2);
        buff[ strlen(buff)] = 0xE0;

        fb_clear_display(view->fb);
        fb_draw_string(view->fb, buff, 0, 0, font_4x7_led);
        fb_flush(view->fb);
}


void _view_render_humidity(view_t* view){
        char buff[6];
        dht_data_t* dht_data = &(view->view_data.dht_data);
        uint8_t humidity_1 = dht_data->humidity / 10;
        uint8_t humidity_2 = dht_data->humidity - humidity_1 * 10;
        sprintf(buff, "%.02d.%.1d%%", humidity_1, humidity_2);

        fb_clear_display(view->fb);
        fb_draw_string(view->fb, buff, 0, 0, font_4x7_led);
        fb_flush(view->fb);
}


void _view_render_time_set(view_t* view){
        char buff[6];
        rtc_date_time_t* dt = &(view->view_data.rtc_date_time);
        bool_t hide = (view->render_cnt & 1);
        if(view->view_data.set_field == SF_HOUR && hide) {
                sprintf(buff, "  :%02d", dt->minute);
        }else if(view->view_data.set_field == SF_MINUTE && hide) {
                sprintf(buff, "%02d:  ", dt->hour);
        }else{
                sprintf(buff, "%02d:%02d", dt->hour, dt->minute);
        }
        fb_clear_display(view->fb);
        fb_draw_string(view->fb, buff, 0, 0, font_4x7_led);
        fb_flush(view->fb);
}

void _view_render_date_set(view_t* view){
        char buff[6];
        rtc_date_time_t* dt = &(view->view_data.rtc_date_time);
        bool_t hide = (view->render_cnt & 1);
        if(view->view_data.set_field == SF_MONTH && hide) {
                sprintf(buff, "  /%02d", dt->month);
        }else if(view->view_data.set_field == SF_DAY && hide) {
                sprintf(buff, "%02d/  ", dt->day);
        }else{
                sprintf(buff, "%02d:%02d", dt->month, dt->day);
        }
        fb_clear_display(view->fb);
        fb_draw_string(view->fb, buff, 0, 0, font_4x7_led);
        fb_flush(view->fb);
}

void _view_render_font_set(view_t* view){
        char buff[6];
        sprintf(buff, "%s", "123456");
        fb_clear_display(view->fb);
        fb_draw_string(view->fb, buff, 0, 0, font_4x7_led);
        fb_flush(view->fb);
}

/////////////////////////////////////////////////////////////////
// helper functions
////////////////////////////////////////////////////////////////


void _view_chain_next(view_t* view, bool_t first){
        view_chain_t chain;
        view_page_t page;
        result_t ret;

        // find next page of chain
        if(first) {
                ret = _view_pgm_read_chain_first(view, &chain);
        }else{
                ret = _view_pgm_find_chain(view, view->view_page_current.page_id, &chain);
        }
        if(ret != ERR_OK) {
                printf("next page of chain not found: code=%d\n", ret);
                return;
        }
        //find page info
        ret = _view_pgm_find_page(view, chain.page_id, &page);
        if(ret != ERR_OK) {
                printf("page not found: page=%d, code=%d\n", chain.page_id, ret);
                return;
        }
        //copy chain data
        memcpy(&view->view_chain_current, &chain, sizeof(view_chain_t));
        //show single page
        _view_show_single_page(view, &page);
}

void _view_show_single_page(view_t* view, view_page_t* view_page){
        memcpy(&view->view_page_current, view_page, sizeof(view_page_t));
        view_page->render_fn(view);
        view->next_page_tick = 0;
        view->render_tick = 0;
        view->render_cnt = 0;
}

result_t _view_pgm_read_chain_first(view_t* view, view_chain_t* view_chain_out){
        memcpy_P(view_chain_out, view->view_chain_head, sizeof(view_chain_t));
        return ERR_OK;
}

result_t _view_pgm_find_chain(view_t* view, uint8_t page_id, view_chain_t* view_chain_out){
        uint8_t page_id_now;
        uint8_t* p = (uint8_t*) view->view_chain_head;
        do {
                page_id_now = pgm_read_byte(p);
                if(page_id_now == page_id) {
                        memcpy_P(view_chain_out, (void*)p, sizeof(view_chain_t));
                        return ERR_OK;
                }else{
                        p += sizeof(view_chain_t);
                }
        } while(page_id_now > 0);

        return ERR_RECORD_NOT_FOUND;
}

result_t _view_pgm_find_page(view_t* view, uint8_t page_id, view_page_t* view_page_out){
        uint8_t page_id_now;
        uint8_t *p = (uint8_t*) view->view_page_head;
        do {
                page_id_now = pgm_read_byte(p);
                if(page_id_now == page_id) {
                        memcpy_P(view_page_out, (void*)p, sizeof(view_page_t));
                        return ERR_OK;
                }else{
                        p += sizeof(view_page_t);
                }
        } while(page_id_now > 0);

        return ERR_RECORD_NOT_FOUND;
}
