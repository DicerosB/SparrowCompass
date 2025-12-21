#include "SparrowCompass.h"

SparrowCompass::SparrowCompass(TwoWire* p_i2c, USBSerial* p_usb){
  this->i2c = p_i2c;
  this->usb = p_usb;
  acc_module, gyr_module, mot_module, mag_module, gps_module = 0;
}

void SparrowCompass::begin(){
  hw_init();
  setup_usb();
  usb->println(hello_world_message);
  i2c->begin();
  scan_for_modules();
  init_modules();
}

void SparrowCompass::init_modules(){

}

void SparrowCompass::work(){
  // handle USB input
  if(usb->available()){
    String buffer = usb->readString();
    buffer.replace("\n", "");
    buffer.replace("\r", "");
    if(buffer == "deadbeef"){
      switch_to_bootloader();
    }else{
      usb->println(buffer);
    }
  }
}

void SparrowCompass::hw_init(){
  pinMode(DEBUG_LED_Pin, OUTPUT);
  pinMode(GPS_nReset_Pin, OUTPUT);
  pinMode(MOT_nEnable_Pin, OUTPUT);
  pinMode(MOT_STEP_Pin, OUTPUT);
  pinMode(MOT_DIR_Pin, OUTPUT);

  digitalWrite(GPS_nReset_Pin, HIGH);
  digitalWrite(MOT_nEnable_Pin, HIGH);
}

void SparrowCompass::setup_usb(){
  // force USB host to re-enumerate
  pinMode(PA12, OUTPUT);
  digitalWrite(PA12, LOW);
  delay(100);
  digitalWrite(PA12, HIGH);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
  usb->begin();
}

void SparrowCompass::scan_for_modules(){
  uint8_t error, address;

  usb->println("Scanning for Modules");

  for(address = 1; address < 127; address++) {
    i2c->beginTransmission(address);
    error = i2c->endTransmission();

    if (!error) {
      switch(address){
        case I2C_ADR_GYROSCOPE:
          #ifdef ENABLE_GYROSCOPE
            gyr_module = 1;
            usb << "Gyroscope found.";
          #endif
        break;
        case I2C_ADR_ACCELEROMETER:
          #ifdef ENABLE_ACCELEROMETER
            acc_module = 1;
            usb->println("Accelerometer found.");
          #endif
        break;
        case I2C_ADR_MAGNETOMETER:
          #ifdef ENABLE_MAGNETOMETER
            mag_module = 1;
            usb->println("Magnetometer found.");
          #endif
        break;
        case I2C_ADR_GPS:
          #ifdef ENABLE_GPS
            gps_module = 1;
            usb->println("GPS found.");
          #endif
        break;
        default:
          usb->println("Unknown module found at address 0x%2H", address);
        break;
      }
    }
    else if (error == 4) {
      usb->println("Communication error at address 0x%2H", address);
    }
  }
}

void SparrowCompass::switch_to_bootloader(){
  usb->println(F("switching to bootloader ..."));
  usb->flush();
	uint32_t* dfu_boot_flag = (uint32_t*)(&_bflag);
	*dfu_boot_flag = DFU_BOOT_FLAG;
	HAL_NVIC_SystemReset();
}