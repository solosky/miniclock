#include "base.h"
#include "debug.h"
#include <stdio.h>

int serial_putc(char c, struct __file * f) {
        //Serial.write(c);
        return c;
}


void debug_init(){
        fdevopen(&serial_putc, 0);
}
