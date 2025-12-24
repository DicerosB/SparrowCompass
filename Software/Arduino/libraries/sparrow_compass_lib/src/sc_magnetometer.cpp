#include "sc_magnetometer.h"

SC_Magnetometer::SC_Magnetometer(TwoWire& p_i2c, uint8_t address){
    this->address = address;
    this->i2cx = &p_i2c;
}

uint8_t SC_Magnetometer::get_id(){
    return (uint32_t)i2cx;
    i2cx->beginTransmission(0x1E);
    i2cx->write(0x4F);
    i2cx->endTransmission();
    i2cx->requestFrom(0x1E, 1);
    return i2cx->read();
    uint8_t id = 0;
    _read_register(&id, WHO_AM_I_M);
    return id;
}

uint8_t SC_Magnetometer::_read_register(uint8_t* buffer, uint8_t reg_addr, uint8_t len){
    uint8_t error = 0;
    
    i2cx->beginTransmission(this->address);
    //i2c->write(reg_addr);
    error = i2cx->endTransmission();
    i2cx->requestFrom(this->address, len);
    //while(i2c->available()) {
        //*buffer = i2c->read();
        //buffer++;
    //}
    return error;
}
uint8_t SC_Magnetometer::_write_register(uint8_t* buffer, uint8_t reg_addr, uint8_t len){
    uint8_t error;
    i2cx->beginTransmission(this->address);
    i2cx->write(reg_addr);
    for(uint8_t i = 0; i < len; i++){
        i2cx->write(*(buffer++));
    }
    error = i2cx->endTransmission();
    return error;
}