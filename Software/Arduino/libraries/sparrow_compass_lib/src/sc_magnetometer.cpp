#include "sc_magnetometer.h"

SC_Magnetometer::SC_Magnetometer(TwoWire* p_i2c, uint8_t address)
    : i2c(p_i2c),
    address(address)
{}

uint8_t SC_Magnetometer::get_id(){
    uint8_t id = 0;
    _read_register(&id, WHO_AM_I_M);
    return id;
}

uint16_t SC_Magnetometer::get_x(){
    uint16_t id = 0;
    _read_register((uint8_t*)&id, OUTX_H_REG_M, 2);
    return id;
}
uint8_t SC_Magnetometer::_read_register(uint8_t* buffer, uint8_t reg_addr, uint8_t len){
    uint8_t error = 0;
    
    i2c->beginTransmission(this->address);
    i2c->write(reg_addr);
    error = i2c->endTransmission();
    i2c->requestFrom(this->address, len);
    while(i2c->available()) {
        *buffer = i2c->read();
        buffer++;
    }
    return error;
}
uint8_t SC_Magnetometer::_write_register(uint8_t* buffer, uint8_t reg_addr, uint8_t len){
    uint8_t error;
    i2c->beginTransmission(this->address);
    i2c->write(reg_addr);
    for(uint8_t i = 0; i < len; i++){
        i2c->write(*(buffer++));
    }
    error = i2c->endTransmission();
    return error;
}