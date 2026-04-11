#ifndef _SC_MOTOR_H_
#define _SC_MOTOR_H_

#include <Arduino.h>

#define CW 1
#define CCW -1
#define MICROSTEPPING 32
#define STEPS_PER_REVOLUTION 200
#define SYNC_SPEED 10 // RPM
#define MAX_SPEED 150
#define ACCELERATION 4 // allowed increase/decrease of RPM per Microstep
#define USTEPS_PER_REVOLUTION MICROSTEPPING*STEPS_PER_REVOLUTION

#ifdef __cplusplus
extern "C" {
#endif




class SC_Motor{
  public:
  SC_Motor(uint8_t nEnable_pin, uint8_t step_pin, uint8_t dir_pin, uint8_t sync_pin);
  // Low Level Motor Control
  void set_speed(uint16_t rpm);
  uint16_t get_speed();
  uint16_t get_current_speed();
  void set_direction_cw();
  void set_direction_ccw();
  bool get_direction();
  bool is_moving();
  void move_inf(); // start rotating infinitely
  void stop(); // stop rotation

  // Advanced Motor procedures
  uint8_t synchronize();
  void move_n_pulses(uint32_t pulses);
  void move_to_heading(uint32_t heading_mdegrees, uint8_t mode=0x03);

  // Interrupt callbacks
  void sync_callback();
  void timer_period_callback();
  
  //utils
  uint16_t rpm2freq(uint16_t rpm);
  uint16_t freq2rpm(uint32_t rpm);
  HardwareTimer *step_timer;
  
  private:
  
  uint8_t nEnable_pin, step_pin, dir_pin, sync_pin;

  uint16_t target_speed;
  int8_t rot_direction;
  int32_t rot_counter;
  volatile bool synchronized;
  bool currently_moving, moving_infinitely;
  volatile uint16_t pulse_counter;
  uint16_t target_counter;
  
};

extern SC_Motor *motor;
void callback_helper();

#ifdef __cplusplus
}
#endif

#endif 