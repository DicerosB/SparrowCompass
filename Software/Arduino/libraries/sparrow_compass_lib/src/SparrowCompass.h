#ifndef _SPARROWCOMPASS_H_
#define _SPARROWCOMPASS_H_

#include <Arduino.h>
#include <Wire.h>
#include <ostream>
#include "Streaming.h"
#include "sc_motor.h"
#include "sc_magnetometer.h"


// boot flag value for entering bootloader mode
#define DFU_BOOT_FLAG 0xDEADBEEF

// enabled device modules
#define ENABLE_MOTOR
#define ENABLE_GYROSCOPE
#define ENABLE_MAGNETOMETER
#define ENABLE_ACCELEROMETER
#define ENABLE_GPS

// I2C addresses of specific modules
#define I2C_ADR_GYROSCOPE 0x6B
#define I2C_ADR_MAGNETOMETER 0x1E
#define I2C_ADR_ACCELEROMETER 0x19
#define I2C_ADR_GPS 0x42

// hardware pinout
#define DEBUG_LED_Pin PC13
#define GPS_nReset_Pin PA0
#define GPS_INT_Pin PA1
#define MAG_DRDY_Pin PA2
#define MAG_INT1_Pin PA3
#define GYR_DRDY_Pin PA4
#define MOT_nEnable_Pin PB11
#define MOT_STEP_Pin PB12
#define MOT_DIR_Pin PB13
#define MOT_SENS_B_Pin PB14
#define MOT_SENS_A_Pin PB15
#define MOT_SENS_B_EXTI_IRQn EXTI15_10_IRQn
#define MOT_SENS_A_EXTI_IRQn EXTI15_10_IRQn

// other config
#define VERBOSE_OUTPUT

// project metadata
#define PROJECT_VERSION "0.0.1"
#define PROJECT_AUTHOR "Petzoldt"

extern int _bflag;
const String hello_world_message = F(
  "\n~~~~~ Sparrow Compass ~~~~~~~\n "
  "Arduino Library\n"
  "Project Version: " PROJECT_VERSION "\n"
  "Author: " PROJECT_AUTHOR
  "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  

#ifdef __cplusplus
extern "C" {
#endif

class SparrowCompass{
  public:
  
  SparrowCompass(TwoWire* p_i2c, USBSerial* p_usb);
  void begin();
  void work();

  private:
  bool acc_module, gyr_module, mot_module, mag_module, gps_module;
  uint32_t loopcounter;
  TwoWire *i2c;
  USBSerial *usb;
  SC_Motor* motor;
  SC_Magnetometer* magnetometer;

  void switch_to_bootloader();
  void scan_for_modules();
  void setup_usb();
  void hw_init();
  void init_modules();
};

#ifdef __cplusplus
}
#endif

#endif 