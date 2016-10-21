#ifndef BUFFER_H
#define BUFFER_H

#include "base.h"

typedef struct _buffer_ {
    byte_t* buff; //buffer mem
    uint8_t size; //total size
    uint8_t pos;  //current write or read position
} buffer_t;



////////////////////////////////////////////////////////////////////
// public functions
///////////////////////////////////////////////////////////////////
// void buffer_init(buffer_t* buffer, byte_t* buff, uint8_t size);
// void buffer_reset(buffer_t* buffer);
// bool_t buffer_can_write(buffer_t* buffer, uint8_t size);
// uint8_t buffer_write(buffer_t* buffer, byte_t one);
// uint8_t buffer_write_buff(buffer_t* buffer, byte_t* buff, uint8_t size);

#endif
