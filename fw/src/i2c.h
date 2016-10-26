#ifndef IIC_H
#define IIC_H
#include "base.h"

#define I2C_RX_BUFF_SIZE 32

enum i2c_state {
        I2C_IDLE, I2C_BUSY
};

#define ERR_I2C_BUSY 1
#define ERR_I2C_BUFF_OVER_FLOW 2


typedef struct __i2c_ {
        uint8_t state;
} i2c_t;


///////////////////////////////////////////////////////////////////////
// public functions
///////////////////////////////////////////////////////////////////////
i2c_t* i2c_default();
void i2c_init(i2c_t* i2c);
result_t i2c_write(i2c_t* i2c, uint8_t addr, byte_t data);
result_t i2c_write_buff(i2c_t* i2c, uint8_t addr, byte_t* buff, uint8_t size);
result_t i2c_read_buff(i2c_t* i2c, uint8_t addr, byte_t* buff, uint8_t size);


#endif
