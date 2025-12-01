#include "SparrowCompass.h"

uint8_t hello_world_message[] =
  "\n~~~~~ Sparrow Compass ~~~~~~~\n "
  " Arduino Library\n"
  "Project Version: " PROJECT_VERSION "\n"
  "Author: " PROJECT_AUTHOR
  "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";


SparrowCompass::SparrowCompass(){
  hw_init(usb);
  delay(1000);
  usb->write(hello_world_message, sizeof(hello_world_message)-1);
}