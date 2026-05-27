#include "sc_magnetometer.h"

SC_Magnetometer::SC_Magnetometer(TwoWire* p_i2c, uint8_t address)
    : i2c(p_i2c),
    address(address)
{}

uint8_t SC_Magnetometer::init(){
    // Configure Magnetometer
    uint8_t config_data[] ={
        0x88, // CFG_REG_A_M : temp compensation, 50 Hz continuous mode
        0x0A, // CFG_REG_B_M 
        0x11, // CFG_REG_C_M 
    };
    write_bytes(config_data, CFG_REG_A_M, 3);
    // load hard-iron offsets
    uint8_t offset_data[] ={
        0x00, 0x00, // OFFSET_X_REG
        0x00, 0x00, // OFFSET_Y_REG 
        0x00, 0x00  // OFFSET_Z_REG 
    };
    write_bytes(offset_data, OFFSET_X_REG_L_M, 6);
    return 0;
}

uint8_t SC_Magnetometer::get_id(){
    uint8_t id = 0;
    read_byte(&id, WHO_AM_I_M);
    return id;
}
uint8_t SC_Magnetometer::get_status(){
    uint8_t status = 0;
    read_byte(&status, STATUS_REG_M);
    return status;
}

void SC_Magnetometer::get_output(int16_t *data){
    uint8_t buffer[6]; // 6 bytes for 3 uint16 values
    read_bytes(buffer, OUTX_L_REG_M, 6);
    data[0] = ((int16_t)buffer[1] << 8) | (int16_t)buffer[0];
    data[1] = ((int16_t)buffer[3] << 8) | (int16_t)buffer[2];
    data[2] = ((int16_t)buffer[5] << 8) | (int16_t)buffer[4];
}


uint8_t SC_Magnetometer::read_byte(uint8_t* buffer, uint8_t reg_addr){
    uint8_t error = 0;
    i2c->beginTransmission(this->address);
    i2c->write(reg_addr);
    error = i2c->endTransmission();
    i2c->requestFrom(this->address, 1);
    while(i2c->available()) *buffer = i2c->read();
    return error;
}

uint8_t SC_Magnetometer::read_uint16(uint16_t* buffer, uint8_t reg_start_addr){
    // note: output LSB equals start address
    uint8_t error = 0;
    i2c->beginTransmission(this->address); 
    i2c->write(reg_start_addr| 0x80); // MSB indicating auto increment read
    error = i2c->endTransmission();
    i2c->requestFrom(this->address, 2);
    *buffer = 0;
    uint8_t cnt = 0;
    while(i2c->available() && cnt<2) {
        *buffer |= (i2c->read())<< (8 * cnt);
        cnt++;
    }
    return error;
}

uint8_t SC_Magnetometer::read_bytes(uint8_t* buffer, uint8_t reg_start_addr, uint8_t len){
    uint8_t error = 0;
    
    i2c->beginTransmission(this->address); 
    i2c->write(reg_start_addr| 0x80); // MSB indicating auto increment read
    error = i2c->endTransmission();
    i2c->requestFrom(this->address, len);
    while(i2c->available()) {
        *buffer = i2c->read();
        buffer++;
    }
    return error;
}

uint8_t SC_Magnetometer::write_byte(uint8_t data, uint8_t reg_addr){
    uint8_t error = 0;
    i2c->beginTransmission(this->address);
    i2c->write(reg_addr);
    i2c->write(data);
    error = i2c->endTransmission();
    return error;
}

uint8_t SC_Magnetometer::write_bytes(uint8_t* buffer, uint8_t reg_start_addr, uint8_t len){
    uint8_t error;
    i2c->beginTransmission(this->address); 
    i2c->write(reg_start_addr | 0x80); // MSB indicating auto increment write
    for(uint8_t i = 0; i < len; i++){
        i2c->write(*(buffer++));
    }
    error = i2c->endTransmission();
    return error;
}