#ifndef _HARDWARE_SETUP_H_
#define _HARDWARE_SETUP_H_

#include <Arduino.h>
#include <Wire.h>

#define DFU_BOOT_FLAG 0xDEADBEEF

extern int _bflag;

void hw_init();
void setup_usb();

void switch_to_bootloader();


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

#endif 
