#include "SparrowCompass.h"

String hello_world_message = F(
  "\n~~~~~ Sparrow Compass ~~~~~~~\n "
  "Arduino Library\n"
  "Project Version: " PROJECT_VERSION "\n"
  "Author: " PROJECT_AUTHOR
  "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");


SparrowCompass::SparrowCompass(){
  
}

void SparrowCompass::begin(){
  // general init
  hw_init();
  setup_usb();
  Serial.println(hello_world_message);

  // init modules
}

void SparrowCompass::work(){
  // handle serial input
  if(Serial.available()){
    String buffer = Serial.readString();
    buffer.replace("\n", "");
    buffer.replace("\r", "");
    if(buffer == "deadbeef"){
      Serial.println(F("switching to bootloader ..."));
      switch_to_bootloader();
    }
    Serial.println(buffer);
  }
  // handle motor
  // handle sensors
  // handle
}
