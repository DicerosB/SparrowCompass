#include "SparrowCompass.h"

// global module pointer definitions
 SC_Motor *motor = nullptr;
 SC_Magnetometer_alternative *magnetometer = nullptr;

SparrowCompass::SparrowCompass(TwoWire* p_i2c, USBSerial* p_usb)
  :
  i2c(p_i2c),
  usb(p_usb),
  loopcounter(0),
  acc_module(0),
  gyr_module(0),
  mot_module(0),
  mag_module(0),
  gps_module(0),
  plotting(0),
  automove(0)
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
    *usb << "magnetometer ID:";
    *usb << magnetometer->get_id() << "\n";
  }
  if(mot_module){
    //motor->synchronize();
  }
  main_interval_timer = millis();
  blink_interval_timer = millis();
}

void SparrowCompass::work(){

  uint32_t loop_time = millis();
  uint16_t loop_duration = 0;

  // main interval
  if (loop_time - main_interval_timer > MAIN_INTERVAL){

    handle_usb();
    magnetometer->get_output(orientation.mag);
    if(plotting){
      orientation.plot(usb);
    }

    main_interval_timer = millis();
  }

  // blink interval
  if (loop_time - blink_interval_timer > BLINK_INTERVAL){
    digitalWrite(DEBUG_LED_Pin, !digitalRead(DEBUG_LED_Pin));

    blink_interval_timer = millis();
  }
  loop_duration = millis() - loop_time;
}

void SparrowCompass::init_modules(){
  #ifdef ENABLE_MOTOR
    if(mot_module){
      pinMode(MOT_nEnable_Pin, OUTPUT);
      digitalWrite(MOT_nEnable_Pin, HIGH);
      motor = new SC_Motor(MOT_nEnable_Pin, MOT_STEP_Pin, MOT_DIR_Pin, MOT_SENS_A_Pin, MOT_SENS_B_Pin, usb);
      #ifdef VERBOSE_OUTPUT
        *usb << "Initialised Motor.\n";
      #endif
  #endif
  #ifdef ENABLE_MAGNETOMETER
    if(mag_module){
      magnetometer = new SC_Magnetometer(i2c, I2C_ADR_MAGNETOMETER);
      magnetometer->init();
      #ifdef VERBOSE_OUTPUT
        *usb << "Initialised Magnetometer.\n";
      #endif
    }
  #endif 
  #ifdef ENABLE_MAGNETOMETER_ALT
    if(mag_module){
      magnetometer = new SC_Magnetometer_alternative(i2c, I2C_ADR_MAGNETOMETER_ALT);
      magnetometer->init();
      #ifdef VERBOSE_OUTPUT
        *usb << "Initialised Magnetometer (alt).\n";
      #endif
    }
  #endif

}

void SparrowCompass::hw_init(){
  pinMode(DEBUG_LED_Pin, OUTPUT);
  pinMode(GPS_nReset_Pin, OUTPUT);
  pinMode(MOT_nEnable_Pin, INPUT_PULLUP);

  digitalWrite(GPS_nReset_Pin, HIGH);

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

    // start decoding usb commands
    if(buffer == "deadbeef"){
      switch_to_bootloader();
    }
    else if(buffer == "h" || buffer == "help"){
      *usb << "----------------------------------\n" \
      << " avaliable commands:\n\n" \
      << "help, h\t\t| show this help text.\n" \
      << "deadbeef\t| enter boot mode for DFU.\n" \
      << "mv CC SS HHHH\t| move needle\n" \
      << "\t\t  C: control byte\n" \
      << "\t\t  S: speed byte\n" \
      << "\t\t  H: heading bytes\n" \
      << "stp\t\t| stop needle\n" \
      << "resume\t\t| continue auto needle movement\n" \
      << "plot\t\t| toggle plot mode\n" \
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
    else if(buffer == "plot"){
      plotting = !plotting;
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
        //case I2C_ADR_GYROSCOPE_ALT:
          gyr_module = 1;
          #ifdef VERBOSE_OUTPUT
          *usb << "Gyroscope found.\n";
          #endif
        break;
        case I2C_ADR_ACCELEROMETER:
        case I2C_ADR_ACCELEROMETER_ALT:
          acc_module = 1;
          #ifdef VERBOSE_OUTPUT
          *usb << "Accelerometer found.\n";
          #endif
        break;
        case I2C_ADR_MAGNETOMETER:
        case I2C_ADR_MAGNETOMETER_ALT:
          mag_module = 1;
          #ifdef VERBOSE_OUTPUT
          *usb << "Magnetometer found.\n";
          #endif
        break;
        case I2C_ADR_GPS:
        case I2C_ADR_GPS_ALT:
          gps_module = 1;
          #ifdef VERBOSE_OUTPUT
          *usb << "GPS found.\n";
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

Orientation::Orientation(){
}

void Orientation::plot(USBSerial* p_usb){
  uint8_t graph_counter = 0;
  *p_usb << "MAG_X:" << mag[0] << ", ";
  *p_usb << "MAG_Y:" << mag[1] << ", ";
  *p_usb << "MAG_Z:" << mag[2] << ", ";
  *p_usb << "ACC_X:" << acc[0] << ", ";
  *p_usb << "ACC_Y:" << acc[1] << ", ";
  *p_usb << "ACC_Z:" << acc[2] << ", ";
  *p_usb << "GYR_X:" << gyr[0] << ", ";
  *p_usb << "GYR_Y:" << gyr[1] << ", ";
  *p_usb << "GYR_Z:" << gyr[2]<< "\n";
}