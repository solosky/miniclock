#ifndef DEBUG_H
#define DEBUG_H
#include "base.h"
#include "usart.h"
#include <stdio.h>


#define DBG_USART_BAUD 115200

void debug_init(usart_t* usart);
void debug_print_buffer(byte_t* buff, uint8_t size);
#endif
