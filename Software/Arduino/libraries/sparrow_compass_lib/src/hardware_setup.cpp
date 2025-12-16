#include "hardware_setup.h"

USBSerial* usb;
uint32_t *dfu_boot_flag;

void hw_init(){
  pinMode(DEBUG_LED_Pin, OUTPUT);
  pinMode(GPS_nReset_Pin, OUTPUT);
  pinMode(MOT_nEnable_Pin, OUTPUT);
  pinMode(MOT_STEP_Pin, OUTPUT);
  pinMode(MOT_DIR_Pin, OUTPUT);
}

void setup_usb(){
    // force USB host to re-enumerate
  pinMode(PA12, OUTPUT);
  digitalWrite(PA12, LOW);
  delay(100);
  digitalWrite(PA12, HIGH);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
  Serial.begin(9600);
  while (!Serial);
}

void switch_to_bootloader(){
  Serial.flush()
	dfu_boot_flag = (uint32_t*)(&_bflag);
	*dfu_boot_flag = DFU_BOOT_FLAG;
	HAL_NVIC_SystemReset();
}

static void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  pinMode(DEBUG_LED_Pin, OUTPUT);
  pinMode(GPS_nReset_Pin, OUTPUT);
  pinMode(MOT_nEnable_Pin, OUTPUT);
  pinMode(MOT_STEP_Pin, OUTPUT);
  pinMode(MOT_DIR_Pin, OUTPUT);


  /*Configure GPIO pins : GPS_INT_Pin MAG_DRDY_Pin MAG_INT1_Pin GYR_DRDY_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : MOT_SENS_B_Pin MOT_SENS_A_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}