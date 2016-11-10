#include "fb.h"
#include "debug.h"
#include "rtc.h"
#include "key.h"
#include "Arduino_FreeRTOS.h"
#include "timers.h"
#include "dht.h"
#include "lux.h"

#include "ctrl.h"

void setup(){
      ctrl_init(ctrl_default());
}

void loop(){
}
