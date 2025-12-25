#ifndef _SC_MAGNETOMETER_H_
#define _SC_MAGNETOMETER_H_

#include "Arduino.h"
#include "Wire.h"
#include <memory>

// magnetometer register map
#define OFFSET_X_REG_L_M    0x45    // R/W
#define OFFSET_X_REG_H_M    0x46    // R/W
#define OFFSET_Y_REG_L_M    0x47    // R/W
#define OFFSET_Y_REG_H_M    0x48    // R/W
#define OFFSET_Z_REG_L_M    0x49    // R/W
#define OFFSET_Z_REG_H_M    0x4A    // R/W
#define WHO_AM_I_M          0x4F    // R
#define CFG_REG_A_M         0x60    // R/W
#define CFG_REG_B_M         0x62    // R/W
#define CFG_REG_C_M         0x63    // R/W
#define INT_CTL_REG_M       0x63    // R/W
#define INT_SOURCE_REG_M    0x64    // R
#define INT_THS_L_REG_M     0x65    // R/W
#define INT_THS_H_REG_M     0x66    // R/W
#define STATUS_REG_M        0x67    // R
#define OUTX_L_REG_M        0x68    // R
#define OUTX_H_REG_M        0x69    // R
#define OUTY_L_REG_M        0x6A    // R
#define OUTY_H_REG_M        0x6B    // R
#define OUTZ_L_REG_M        0x6C    // R
#define OUTZ_H_REG_M        0x6D    // R


#ifdef __cplusplus
extern "C" {
#endif

class SC_Magnetometer{
  public:
  
  SC_Magnetometer(TwoWire* p_i2c, uint8_t address);
  uint8_t get_id();
  uint16_t get_x();

  private:
  uint8_t address;
  TwoWire* i2c;
  uint8_t _read_register(uint8_t* buffer, uint8_t reg_addr, uint8_t len = 1);
  uint8_t _write_register(uint8_t* buffer, uint8_t reg_addr, uint8_t len = 1);
};

#ifdef __cplusplus
}
#endif

#endif 