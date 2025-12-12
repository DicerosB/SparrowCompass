#ifndef _SPARROWCOMPASS_H_
#define _SPARROWCOMPASS_H_

#include <Arduino.h>
#include "hardware_setup.h"

#define PROJECT_VERSION "0.0.1"
#define PROJECT_AUTHOR "M. Petzoldt"

#ifdef __cplusplus
extern "C" {
#endif

class SparrowCompass{
  public:
  USBSerial *usb;
  SparrowCompass();
  
  private:
};

#ifdef __cplusplus
}
#endif

#endif 