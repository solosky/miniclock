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


void debug_print_buffer(byte_t* buff, uint8_t size) {
        byte_t *p = buff;
        byte_t c = 0;
        while (p < buff + size) {
                printf("%.2X ", (*p));
                if (((++c) % 8) == 0) {
                        printf("\n");
                }
                p++;
        }
        printf("\n");
}
