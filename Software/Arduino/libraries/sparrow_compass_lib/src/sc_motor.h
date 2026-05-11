#ifndef _SC_MOTOR_H_
#define _SC_MOTOR_H_

#include <Arduino.h>

#include "Streaming.h"
#define DEBUG_LED_Pin PC13

// motor independent defines
#define CW 0
#define CCW 1
#define SPEED_EVAL_FREQ 50 // Rate (Hz) at which motor speed is adjusted for acceleration control

// motor specific defines
#define MICROSTEPPING 32
#define STEPS_PER_REVOLUTION 200
#define MAX_ACCELERATION 8000 // Hz/s

// calculations
#define MAX_ACCELERATION_PER_EVAL_T_Q8 ((MAX_ACCELERATION << 8) / SPEED_EVAL_FREQ) // 6400 (>> 8 = 25 Hz/50ms )
#define USTEPS_PER_REVOLUTION (MICROSTEPPING*STEPS_PER_REVOLUTION) // 6400
#define HEADING_FRACTIONS_PER_MICROSTEP_Q9 (((0x10000 << 9) + (USTEPS_PER_REVOLUTION / 2)) / USTEPS_PER_REVOLUTION) // 5243 (>> 9 = 10,24)
#define RPM_HZ_CONVERSION_FACTOR_Q8 (((USTEPS_PER_REVOLUTION << 8) + 30) / 60) // 27307 (>> 8 = 106,667)


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t mot_instruction_t; // MSB <|  control | speed | heading_msb | heading_lsb |> LSB

class SC_Motor{
  public:
  SC_Motor(uint8_t nEnable_pin, uint8_t step_pin, uint8_t dir_pin, uint8_t sync_pin_a, uint8_t sync_pin_b, USBSerial* p_usb);
  
  uint16_t get_heading();
  bool is_moving();

  // basic movement
  void move(uint8_t control, uint8_t speed, uint16_t heading);
  void stop(); // stop rotation

  // motor procedures
  void animate(uint32_t *instructions);
  uint8_t synchronize();

  // Interrupt callbacks
  void sync_callback();
  void step_timer_period_callback();
  void speed_timer_period_callback();

  // Timer objects
  HardwareTimer *step_timer;
  HardwareTimer *speed_timer;

  USBSerial *usb;

  private:
  
  uint8_t nEnable_pin, step_pin, dir_pin, sync_pin_a, sync_pin_b;

  uint8_t target_rpm; // in rounds per minute
  bool rot_direction; // 0: CW 1:CCW
  uint16_t target_counter; // 0..0xFFFF mapped to one full rotation

  volatile uint16_t pulse_counter;
  volatile bool synchronized;
  bool currently_moving, moving_infinitely;
  void set_pwm_f(uint16_t speed_hz);
  
};

extern SC_Motor *motor; // pointer to global motor instance

// Interrupt callback helper
void step_callback_helper();
void speed_callback_helper();

#ifdef __cplusplus
}
#endif

#endif 