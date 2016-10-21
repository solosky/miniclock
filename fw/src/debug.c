#include "base.h"
#include "debug.h"
#include "usart.h"

int serial_putc(char c, struct __file * f) {
      usart_write(usart_default(), (byte_t) c);
        return c;
}


void debug_init(){
        usart_init(usart_default(), DBG_USART_BAUD);
        fdevopen(&serial_putc, 0);
}
