#ifndef _HARDWARE_SETUP_H_
#define _HARDWARE_SETUP_H_

#include <Arduino.h>
#include <Wire.h>
#include <USBSerial.h>

#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)8000000U) /*!< Default value of the External oscillator in Hz.
                                                This value can be provided and adapted by the user application. */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000U) /*!< Default value of the Internal oscillator in Hz.
                                                This value can be provided and adapted by the user application. */
#endif /* HSI_VALUE */

#define USER_VECT_TAB_ADDRESS
#define FLASH_BASE 0x8008000
#define DFU_BOOT_FLAG 0xDEADBEEF



void hw_init(USBSerial* usb_ref);


static void USB_Device_Init();
static void GPIO_Init(void);
void USB_CDC_RxHandler(uint8_t* Buf, uint32_t Len);
void switch_to_bootloader();
int _write(int fd, char *ptr, int len);
//static void MX_I2C1_Init(void);
void SystemClock_Config(void);

extern int _bflag;

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
