#ifndef TIMER_H
#define TIMER_H
#include "base.h"
#include "timers.h"

typedef TimerHandle_t timer_t;
typedef void (*timer_fn_t)(void*);

timer_t timer_new(const char* name, uint16_t tick, timer_fn_t fn, bool_t auto_reload);
void timer_kill(timer_t timer);

#endif
