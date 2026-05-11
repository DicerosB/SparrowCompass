#include "SparrowCompass.h"

// global module pointer definitions
 SC_Motor *motor = nullptr;
 SC_Magnetometer *magnetometer = nullptr;

SparrowCompass::SparrowCompass(TwoWire* p_i2c, USBSerial* p_usb)
  :
  i2c(p_i2c),
  usb(p_usb),
  loopcounter(0),
  acc_module(0),
  gyr_module(0),
  mot_module(0),
  mag_module(0),
  gps_module(0)
{}

void SparrowCompass::begin(){
  hw_init();
  setup_usb();
  *usb << hello_world_message;
  i2c->begin();
  scan_for_modules();
  init_modules();

  // module test
  if(mag_module){
    *usb << "magnetometer ID:" << magnetometer->get_id() << "\n";
  }
  if(mot_module){
    //motor->synchronize();
  }
}

void SparrowCompass::work(){
  handle_usb();

  //digitalWrite(DEBUG_LED_Pin, 1);
  //delay(1000);
  //digitalWrite(DEBUG_LED_Pin, 0);
  digitalWrite(MOT_nEnable_Pin, LOW);
  for(int i = 0; i < 6400; i++){
    digitalWrite(MOT_STEP_Pin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(MOT_STEP_Pin, LOW);
    delayMicroseconds(2000);
  }
  digitalWrite(MOT_nEnable_Pin, HIGH);
  delay(10000);
  loopcounter++;
}

void SparrowCompass::init_modules(){
  if(mot_module){
    #ifdef VERBOSE_OUTPUT
    *usb << "Initialising Motor.\n";
    #endif
    motor = new SC_Motor(MOT_nEnable_Pin, MOT_STEP_Pin, MOT_DIR_Pin, MOT_SENS_A_Pin, MOT_SENS_B_Pin, usb);
    *usb << "Initialised Motor.\n";
  }
  if(mag_module){
    #ifdef VERBOSE_OUTPUT
    *usb << "Initialising Magnetometer.\n";
    #endif
    magnetometer = new SC_Magnetometer(i2c, I2C_ADR_MAGNETOMETER);
  }
}

void SparrowCompass::hw_init(){
  pinMode(DEBUG_LED_Pin, OUTPUT);
  pinMode(GPS_nReset_Pin, OUTPUT);

  digitalWrite(GPS_nReset_Pin, HIGH);
  digitalWrite(MOT_nEnable_Pin, HIGH);

  //debug
  pinMode(MOT_STEP_Pin, OUTPUT);
  pinMode(MOT_nEnable_Pin, OUTPUT);
}

void SparrowCompass::setup_usb(){
  // force USB host to re-enumerate
  pinMode(PA12, OUTPUT);
  digitalWrite(PA12, LOW);
  delay(100);
  digitalWrite(PA12, HIGH);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
  usb->begin();
  uint32_t timeout = millis()+10000;
  while(!*usb){
    delay(100);
    if(millis() > timeout) break; // no usb connected
  }
}
void SparrowCompass::handle_usb(){
  // handle USB input
  if(usb->available()){
    String buffer = usb->readString();
    buffer.replace("\n", "");
    buffer.replace("\r", "");


    if(buffer == "deadbeef"){
      switch_to_bootloader();
    }
    else if(buffer == "h" || buffer == "help"){
      *usb << "----------------------------------\n" \
      << " avaliable commands:\n\n" \
      << "help, h\t\t| show this help text.\n" \
      << "deadbeef\t| enter boot mode for DFU.\n" \
      << "mv CC SS HHHH\t| move motor\n" \
      << "\t\t  C: control byte\n" \
      << "\t\t  S: speed byte\n" \
      << "\t\t  H: heading bytes\n" \
      << "----------------------------------\n\n";
    }
    else if(buffer.startsWith("mv ") && buffer.length() == 13){
      uint8_t control = strtol(buffer.substring(3,5).c_str(), NULL, 16);
      uint8_t speed = strtol(buffer.substring(6, 8).c_str(), NULL, 16);
      uint16_t heading = strtol(buffer.substring(9).c_str(), NULL, 16);
      *usb << "moving motor to "<< heading <<" with "<< speed <<" rpm.\n";
      motor->move(control, speed, heading);
    }
    else if(buffer == "stp"){
      motor->stop();
      *usb << "stopped motor\n";
    }
    else{ // echo
      *usb << buffer << "\n";
    }
  }
}

void SparrowCompass::scan_for_modules(){
  uint8_t error, address;
  
  #ifdef ENABLE_MOTOR
    mot_module = 1;
    #ifdef VERBOSE_OUTPUT
    *usb << "Enabled Motor.\n";
    #endif
  #endif

  #ifdef VERBOSE_OUTPUT
  *usb << "Scanning for Modules...\n";
  #endif
  for(address = 1; address < 127; address++) {
    i2c->beginTransmission(address);
    error = i2c->endTransmission();

    if (!error) {
      switch(address){
        case I2C_ADR_GYROSCOPE:
          #ifdef ENABLE_GYROSCOPE
            gyr_module = 1;
            #ifdef VERBOSE_OUTPUT
            *usb << "Gyroscope found.\n";
            #endif
          #endif
        break;
        case I2C_ADR_ACCELEROMETER:
          #ifdef ENABLE_ACCELEROMETER
            acc_module = 1;
            #ifdef VERBOSE_OUTPUT
            *usb << "Accelerometer found.\n";
            #endif
          #endif
        break;
        case I2C_ADR_MAGNETOMETER:
          #ifdef ENABLE_MAGNETOMETER
            mag_module = 1;
            #ifdef VERBOSE_OUTPUT
            *usb << "Magnetometer found.\n";
            #endif
          #endif
        break;
        case I2C_ADR_GPS:
          #ifdef ENABLE_GPS
            gps_module = 1;
            #ifdef VERBOSE_OUTPUT
            *usb << "GPS found.\n";
            #endif
          #endif
        break;
        default:
          #ifdef VERBOSE_OUTPUT
          *usb << "Unknown module found at address 0x" << _HEX(address) << "\n";
          #endif
        break;
      }
    }
    else if (error == 4) {
      #ifdef VERBOSE_OUTPUT
      *usb << "Communication error at address 0x" << _HEX(address) << "\n";
      #endif
    }
  }
}

void SparrowCompass::switch_to_bootloader(){
  *usb << "switching to bootloader ...\n";
  usb->flush();
	uint32_t* dfu_boot_flag = (uint32_t*)(&_bflag);
	*dfu_boot_flag = DFU_BOOT_FLAG;
	HAL_NVIC_SystemReset();
}