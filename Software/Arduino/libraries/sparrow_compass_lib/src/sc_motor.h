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
  static uint16_t get_current_speed();
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


  //utils
  static uint16_t rpm2freq(uint16_t rpm);
  static uint16_t freq2rpm(uint32_t rpm);

  private:
  static HardwareTimer *step_timer;
  static uint8_t nEnable_pin, step_pin, dir_pin, sync_pin;

  static uint16_t target_speed;
  static int8_t rot_direction;
  static int32_t rot_counter;
  static volatile bool synchronized;
  static bool currently_moving, moving_infinitely;
  static volatile uint16_t pulse_counter;
  static uint16_t target_counter;

  // Interrupt callbacks
  static void sync_callback();
  static void timer_period_callback();
};

#ifdef __cplusplus
}
#endif

#endif 