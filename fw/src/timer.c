#include "timer.h"

timer_t timer_new(const char* name, uint16_t tick, timer_fn_t fn, bool_t auto_reload){
      printf("=======\n");
        TimerHandle_t handle = xTimerCreate(name, tick, auto_reload, (void *)0, fn);
        xTimerStart( handle, 0 );
        printf("--------\n");
        return (timer_t) handle;
}


void timer_kill(timer_t timer){
        xTimerDelete((TimerHandle_t) timer, 0);
}
