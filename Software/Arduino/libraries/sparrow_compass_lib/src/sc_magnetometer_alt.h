#ifndef _SC_MAGNETOMETER_ALT_H_
#define _SC_MAGNETOMETER_ALT_H_

#include "Arduino.h"
#include "Wire.h"
#include "Streaming.h"

// magnetometer register map
#define OFFSET_X_REG_L_M 0x05   // R/W
#define OFFSET_X_REG_H_M 0x06   // R/W
#define OFFSET_Y_REG_L_M 0x07   // R/W
#define OFFSET_Y_REG_H_M 0x08   // R/W
#define OFFSET_Z_REG_L_M 0x09   // R/W
#define OFFSET_Z_REG_H_M 0x0A   // R/W
#define WHO_AM_I_M 0x0F         // R
#define CTRL_REG1_M 0x20        // R/W
#define CTRL_REG2_M 0x21        // R/W
#define CTRL_REG3_M 0x22        // R/W
#define CTRL_REG4_M 0x23        // R/W
#define CTRL_REG5_M 0x24        // R/W
#define STATUS_REG_M 0x27       // R
#define OUT_X_L_M 0x28          // R
#define OUT_X_H_M 0x29          // R
#define OUT_Y_L_M 0x2A          // R
#define OUT_Y_H_M 0x2B          // R
#define OUT_Z_L_M 0x2C          // R
#define OUT_Z_H_M 0x2D          // R
#define INT_CFG_M 0x30          // R/W
#define INT_SRC_M 0x31          // R
#define INT_THS_L_M 0x32        // R
#define INT_THS_H_M 0x33        // R


#ifdef __cplusplus
extern "C" {
#endif

class SC_Magnetometer_alternative{
  public:
  
  SC_Magnetometer_alternative(TwoWire* p_i2c, uint8_t address);
  uint8_t init();
  uint8_t get_id();
  uint8_t get_status();
  void get_output(int16_t *data);

  private:
  uint8_t address, status;
  TwoWire* i2c;
  uint8_t read_byte(uint8_t* buffer, uint8_t reg_addr);
  uint8_t read_uint16(uint16_t* buffer, uint8_t reg_start_addr);
  uint8_t read_bytes(uint8_t* buffer, uint8_t start_reg_addr, uint8_t len);
  uint8_t write_byte(uint8_t data, uint8_t start_reg_addr);
  uint8_t write_bytes(uint8_t* buffer, uint8_t reg_addr, uint8_t len);
 
};

#ifdef __cplusplus
}
#endif

#endif 