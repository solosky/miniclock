#ifndef TM_H
#define TM_H

#include "base.h"


//tm defines
typedef struct _tm_{
  uint8_t sclk_pin;
  uint8_t din_pin;
} tm_t;

// constants
#define TM_CMD_BRIGHNESS 0b10001000
#define TM_CMD_ADDR_INC 0b01000000
#define TM_CMD_ADDR_SET 0b01000100
#define TM_ADDR_BASE 0b11000000


/////////////////////////////////////////////////////////
///public functions
/////////////////////////////////////////////////////////
void tm_init(tm_t* tm, uint8_t sclk_pin, uint8_t din_pin);
void tm_set_brightless(tm_t* tm, uint8_t level);
void tm_display(tm_t* tm, uint8_t* mem, uint8_t size);


/////////////////////////////////////////////////////////
//private functions
/////////////////////////////////////////////////////////
//tm protocol functions
void _tm_start(tm_t* tm);
void _tm_stop(tm_t* tm);
void _tm_shift_out(tm_t* tm, uint8_t data);

//cmd functions
void _tm_cmd(tm_t* tm, uint8_t cmd);
void _tm_write(tm_t* tm, uint8_t cmd, uint8_t data);
void _tm_write_buff(tm_t* tm, uint8_t cmd, uint8_t* buff, uint8_t len);

#endif
