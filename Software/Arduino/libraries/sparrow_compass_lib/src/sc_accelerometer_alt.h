#ifndef _SC_ACCELEROMETER_ALT_H_
#define _SC_ACCELEROMETER_ALT_H_

#include "Arduino.h"
#include "Wire.h"
#include "Streaming.h"

// accelerometer register map
#define ACT_THS 0x04            // R/W
#define ACT_DUR 0x05            // R/W
#define INT_GEN_CFG_XL 0x06     // R/W
#define INT_GEN_THS_X_XL 0x07   // R/W
#define INT_GEN_THS_Y_XL 0x08   // R/W
#define INT_GEN_THS_Z_XL 0x09   // R/W
#define INT_GEN_DUR_XL 0x0A     // R/W
#define REFERENCE_G 0x0B        // R/W
#define INT1_CTRL 0x0C          // R/W
#define INT2_CTRL 0x0D          // R/W
#define WHO_AM_I 0x0F           // R
#define CTRL_REG1_G 0x10        // R/W
#define CTRL_REG2_G 0x11        // R/W
#define CTRL_REG3_G 0x12        // R/W
#define ORIENT_CFG_G 0x13       // R/W
#define INT_GEN_SRC_G 0x14      // R
#define OUT_TEMP_L 0x15         // R
#define OUT_TEMP_H 0x16         // R
#define STATUS_REG 0x17         // R
#define OUT_X_L_G 0x18          // R
#define OUT_X_H_G 0x19          // R
#define OUT_Y_L_G 0x1A          // R
#define OUT_Y_H_G 0x1B          // R
#define OUT_Z_L_G 0x1C          // R
#define OUT_Z_H_G 0x1D          // R
#define CTRL_REG4 0x1E          // R/W
#define CTRL_REG5_X 0x1F        // R/W
#define CTRL_REG6_XL 0x20       // R/W
#define CTRL_REG7_XL 0x21       // R/W
#define CTRL_REG8 0x22          // R/W
#define CTRL_REG9 0x23          // R/W
#define CTRL_REG10 0x24         // R/W
#define INT_GEN_SRC_XL 0x26     // R
#define STATUS_REG 0x27         // R
#define OUT_X_L_XL 0x28         // R
#define OUT_X_H_XL 0x29         // R
#define OUT_Y_L_XL 0x2A         // R
#define OUT_Y_H_XL 0x2B         // R
#define OUT_Z_L_XL 0x2C         // R
#define OUT_Z_H_XL 0x2D         // R
#define FIFO_CTRL 0x2E          // R/W
#define FIFO_SRC 0x2F           // R
#define INT_GEN_CFG_G 0x30      // R/W
#define INT_GEN_THS_XH_G 0x31   // R/W
#define INT_GEN_THS_XL_G 0x32   // R/W
#define INT_GEN_THS_YH_G 0x33   // R/W
#define INT_GEN_THS_YL_G 0x34   // R/W
#define INT_GEN_THS_ZH_G 0x35   // R/W
#define INT_GEN_THS_ZL_G 0x36   // R/W
#define INT_GEN_DUR_G 0x37      // R/W


#ifdef __cplusplus
extern "C" {
#endif

class SC_Accelerometer_alternative{
  public:
  
  SC_Accelerometer_alternative(TwoWire* p_i2c, uint8_t address);
  uint8_t init();
  uint8_t get_id();
  uint8_t get_status();
  void get_output(int16_t *acc_data, int16_t *gyr_data);

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