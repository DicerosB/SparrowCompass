#ifndef _SC_MOTOR_H_
#define _SC_MOTOR_H_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

class SC_Motor{
  public:
  
  SC_Motor(uint8_t nEnable_pin, uint8_t step_pin, uint8_t dir_pin);
  void set_speed(uint16_t pulsefreq);
  void set_direction(bool clockwise);
  void start();
  void stop();
  void move_n_pulses(uint32_t pulses);

  private:
  uint8_t nEnable_pin, step_pin, dir_pin;
  uint16_t speed;
  bool direction;
  uint32_t pulsecounter;
  void pwm_callback();
};

#ifdef __cplusplus
}
#endif

#endif 