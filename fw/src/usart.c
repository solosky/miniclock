#include "usart.h"

usart_t _usart0;

usart_t* usart_default(){
        return &_usart0;
}

void usart_init(usart_t* usart, uint32_t baud){
        // Try u2x mode first
        uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
        UCSR0A = 1 << U2X0;

// assign the baud_setting, a.k.a. ubrr (USART Baud Rate Register)
        UBRR0H = baud_setting >> 8;
        UBRR0L = baud_setting;

        UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8bits  1stop

// enable RX and TX
        sbi(UCSR0B, RXEN0);
        sbi(UCSR0B, TXEN0);
//sbi(UCSR0B, RXCIE0);
//cbi(UCSR0B, UDRIE0);
}
void usart_write(usart_t* usart, byte_t data){
        // waiting data register empty
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = data;
}


void usart_write_buff(usart_t* usart, byte_t* buff, uint8_t size){
        for(uint8_t i = 0; i<size; i++) {
                usart_write(usart, buff[i]);
        }
}
