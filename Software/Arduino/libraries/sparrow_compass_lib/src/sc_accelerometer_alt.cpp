#include "sc_accelerometer_alt.h"

SC_Accelerometer_alternative::SC_Accelerometer_alternative(TwoWire* p_i2c, uint8_t address)
    : i2c(p_i2c),
    address(address)
{}

uint8_t SC_Accelerometer_alternative::init(){
    // Configure gyroscope
    uint8_t config_data_g[] ={
        0b01100001, // CTRL_REG1_G
        0b00000000, // CTRL_REG2_G
        0b00000000, // CTRL_REG3_G
    };
    write_bytes(config_data_g, CTRL_REG1_G, 3);
   // Configure accelerometer
    uint8_t config_data_a[] ={
        0b00111000, // CTRL_REG4
        0b00111000, // CTRL_REG5_XL
        0b01100000, // CTRL_REG6_XL
        0b00000000, // CTRL_REG7_XL
        0b01000000, // CTRL_REG8
        0b00000000, // CTRL_REG9
        0b00000000, // CTRL_REG10
    };
    write_bytes(config_data_a, CTRL_REG4, 7);
    return 0;
}

uint8_t SC_Accelerometer_alternative::get_id(){
    uint8_t id = 0;
    read_byte(&id, WHO_AM_I);
    return id;
}
uint8_t SC_Accelerometer_alternative::get_status(){
    uint8_t status = 0;
    read_byte(&status, STATUS_REG);
    return status;
}

void SC_Accelerometer_alternative::get_output(int16_t *acc_data, int16_t *gyr_data){
    uint8_t buffer[6]; // 6 bytes for 3 uint16 values
    read_bytes(buffer, OUT_X_L_XL, 6);
    acc_data[0] = ((int16_t)buffer[1] << 8) | (int16_t)buffer[0];
    acc_data[1] = ((int16_t)buffer[3] << 8) | (int16_t)buffer[2];
    acc_data[2] = ((int16_t)buffer[5] << 8) | (int16_t)buffer[4];
    read_bytes(buffer, OUT_X_L_G, 6);
    gyr_data[0] = ((int16_t)buffer[1] << 8) | (int16_t)buffer[0];
    gyr_data[1] = ((int16_t)buffer[3] << 8) | (int16_t)buffer[2];
    gyr_data[2] = ((int16_t)buffer[5] << 8) | (int16_t)buffer[4];
}


uint8_t SC_Accelerometer_alternative::read_byte(uint8_t* buffer, uint8_t reg_addr){
    uint8_t error = 0;
    i2c->beginTransmission(this->address);
    i2c->write(reg_addr);
    error = i2c->endTransmission();
    i2c->requestFrom(this->address, 1);
    while(i2c->available()) *buffer = i2c->read();
    return error;
}

uint8_t SC_Accelerometer_alternative::read_uint16(uint16_t* buffer, uint8_t reg_start_addr){
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

uint8_t SC_Accelerometer_alternative::read_bytes(uint8_t* buffer, uint8_t reg_start_addr, uint8_t len){
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

uint8_t SC_Accelerometer_alternative::write_byte(uint8_t data, uint8_t reg_addr){
    uint8_t error = 0;
    i2c->beginTransmission(this->address);
    i2c->write(reg_addr);
    i2c->write(data);
    error = i2c->endTransmission();
    return error;
}

uint8_t SC_Accelerometer_alternative::write_bytes(uint8_t* buffer, uint8_t reg_start_addr, uint8_t len){
    uint8_t error;
    i2c->beginTransmission(this->address); 
    i2c->write(reg_start_addr | 0x80); // MSB indicating auto increment write
    for(uint8_t i = 0; i < len; i++){
        i2c->write(*(buffer++));
    }
    error = i2c->endTransmission();
    return error;
}