#include "i2c.h"
#include "buffer.h"
#include "utility/twi.h"

i2c_t _i2c0;


i2c_t* i2c_default(){
        return &_i2c0;
}

void i2c_init(i2c_t* i2c){
        i2c->state = I2C_IDLE;
}

result_t i2c_write(i2c_t* i2c, uint8_t addr, byte_t data){
        // if(i2c->state != I2C_IDLE) {
        //         return ERR_I2C_BUSY;
        // }
        twi_init();
        i2c->state = I2C_BUSY;
        uint8_t ret = twi_writeTo(addr, &data, 1, 1, 1);
        //printf("i2c_write:%d %x\n", ret, addr);
        i2c->state = I2C_IDLE;
        twi_disable();
        return ret == 0 ? ERR_OK : ERR_FAILED;
}


result_t i2c_write_buff(i2c_t* i2c, uint8_t addr, byte_t* buff, uint8_t size){
        // if(i2c->state != I2C_IDLE) {
        //         return ERR_I2C_BUSY;
        // }
        i2c->state = I2C_BUSY;
        twi_init();
        uint8_t ret = twi_writeTo(addr, buff, size, 1, 1);
        i2c->state = I2C_IDLE;
        twi_disable();
        return ret == 0 ? ERR_OK : ERR_FAILED;
}


result_t i2c_read_buff(i2c_t* i2c, uint8_t addr, byte_t* buff, uint8_t size){
        // if(i2c->state != I2C_IDLE) {
        //         return ERR_I2C_BUSY;
        // }
        i2c->state = I2C_BUSY;
        twi_init();
        uint8_t nread = twi_readFrom(addr, buff, size, 1);
        //printf("i2c_read: %d\n", nread);
        i2c->state = I2C_IDLE;
        twi_disable();
        return nread == size ? ERR_OK : ERR_FAILED;
}
