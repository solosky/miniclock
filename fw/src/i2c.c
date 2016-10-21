#include "i2c.h"
#include "buffer.h"
#include "utility/twi.h"

i2c_t _i2c0;


i2c_t* i2c_default(){
  return &_i2c0;
}

void i2c_init(i2c_t* i2c, uint32_t clk_frq){
    i2c->clk_frq = clk_frq;
    i2c->state = I2C_IDLE;
  //set bit rate register
    TWBR = ((F_CPU / clk_frq) - 16) / 2;
    twi_init();

}

result_t i2c_write(i2c_t* i2c, uint8_t addr, byte_t data){
  if(i2c->state == I2C_IDLE){
    return ERR_I2C_BUSY;
  }
  i2c->state = I2C_BUSY;
  uint8_t ret = twi_writeTo(addr, &(data), 1, 1, 1);
  i2c->state = I2C_IDLE;
  return ret == 0 ? ERR_OK : ERR_FAILED;
}


result_t i2c_write_buff(i2c_t* i2c, uint8_t addr, byte_t* buff, uint8_t size){
  if(i2c->state == I2C_IDLE){
    return ERR_I2C_BUSY;
  }
  i2c->state = I2C_BUSY;
  uint8_t ret = twi_writeTo(addr, buff, size, 1, 1);
  i2c->state = I2C_IDLE;
  return ret == 0 ? ERR_OK : ERR_FAILED;
}


result_t i2c_read_buff(i2c_t* i2c, uint8_t addr, byte_t* buff, uint8_t size){
  if(i2c->state == I2C_IDLE){
    return ERR_I2C_BUSY;
  }
  i2c->state = I2C_BUSY;
  uint8_t ret = twi_readFrom(addr, buff, size, 1);
  i2c->state = I2C_IDLE;
  return ret == 0 ? ERR_OK : ERR_FAILED;
}
