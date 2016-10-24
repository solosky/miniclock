#include "view.h"

view_t _view0;

//////////////////////////////////////////////////////////////////////
// static defines
//////////////////////////////////////////////////////////////////////
// view pages
//page id, render_interval, render fn
const view_page_t _view_pages[] PROGMEM = {
        {PG_TIME, 500, _view_render_time},
        {PG_TEMP, 1000, _view_render_temp},
        {PG_HUMIDITY, 1000, _view_render_humidity},
        {PG_DATE, 0, _view_render_date},
        {0,0,0}
};

//view chains
//page id, next page id, next delay
const view_chain_t _view_chain_default[] PROGMEM = {
        {PG_TIME, PG_DATE, 5000},
        {PG_DATE, PG_TEMP, 3000},
        {PG_TEMP, PG_HUMIDITY, 3000},
        {PG_HUMIDITY, PG_TIME, 3000},
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


void view_show_page(view_t* view, uint8_t page_id){
        view_page_t page;
        result_t ret;

        //find page info
        ret = _view_pgm_find_page(view, page_id, &page);
        if(ret != ERR_OK) {
                printf("page not found: page=%d, code=%d\n", page_id, ret);
                return;
        }

        _view_stop_chain(view);
        _view_show_single_page(view, &page);
}

void view_show_chain(view_t* view, const view_chain_t* head){
        _view_stop_chain(view);
        view->view_chain_head = head;
        _view_chain_next(view, true);

}


/////////////////////////////////////////////////////////////////
// render functions
////////////////////////////////////////////////////////////////
void _view_render_time(view_t* view){
        char buff[6];
        //rtc_date_time_t* dt = &(view->view_data.rtc_date_time);
        rtc_time_t * tm = &(rtc_default()->rtc_time);
        char dot = (view->render_tick & 1) ? ' ' : ':';
        sprintf(buff, "%02d%c%02d", tm->minute, dot,  tm->second);
        fb_clear_display(view->fb);
        fb_draw_string(view->fb, buff, 0, 0, font_4x7_led);
        fb_flush(view->fb);
}

void _view_render_date(view_t* view){
  char buff[6];

  sprintf(buff, "%s", "123");
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

/////////////////////////////////////////////////////////////////
// helper functions
////////////////////////////////////////////////////////////////

void _view_stop_chain(view_t* view){
        if(view->next_page_timer) {
                timer_kill(view->next_page_timer);
        }
        view->view_chain_head = NULL;
        view->next_page_timer = NULL;
        zero(&view->view_chain_current, sizeof(view->view_chain_current));
}

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

        //next page timer
        if(chain.next_page_delay > 0) {
                view->next_page_timer = timer_new("vn",
                                                  chain.next_page_delay / portTICK_PERIOD_MS,
                                                  _view_timer_next_page, false);
                if(view->next_page_timer == NULL) {
                        printf("create next page timer failed!\n");
                }
        }
}

void _view_show_single_page(view_t* view, view_page_t* view_page){
        //clear render timer
        if(view->render_tick_timer != NULL) {
                timer_kill(view->render_tick_timer);
                view->render_tick_timer = NULL;
        }

        //copy page data to view
        memcpy(&view->view_page_current, view_page, sizeof(view_page_t));

        // render it
        view_page->render_fn(view);


        // set render timer
        if(view_page->render_interval > 0) {
                view->render_tick_timer = timer_new("vr",
                                                    view_page->render_interval / portTICK_PERIOD_MS,
                                                    _view_timer_render_page, true);
                if(view->render_tick_timer == NULL) {
                        printf("create render timer failed\n");
                }
        }

}

void _view_timer_next_page(void* p){
        _view_chain_next(view_default(), false);
}

void _view_timer_render_page(void* p){
        view_t* view = view_default();
        view->render_tick ++;
        view->view_page_current.render_fn(view);
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
