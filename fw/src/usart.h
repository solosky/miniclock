#ifndef USART_H
#define USART_H

#include "base.h"

typedef struct _usart_{
		uint32_t baud;
} usart_t;



/////////////////////////////////////////////////////////////
// public functions
////////////////////////////////////////////////////////////
usart_t* usart_default();
void usart_init(usart_t* usart, uint32_t baud);
void usart_write(usart_t* usart, byte_t data);
void usart_write_buff(usart_t* usart, byte_t* buff, uint8_t size);
#endif
