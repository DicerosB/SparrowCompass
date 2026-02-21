#include "sc_motor.h"

SC_Motor::SC_Motor(uint8_t nEnable_pin, uint8_t step_pin, uint8_t dir_pin){
    this->nEnable_pin = nEnable_pin;
    this->step_pin = step_pin;
    this->dir_pin = dir_pin;
    pinMode(nEnable_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);

    digitalWrite(nEnable_pin, HIGH);
    digitalWrite(dir_pin, LOW);

    TIM_TypeDef *Instance = TIM10;
    this->step_timer = new HardwareTimer(Instance);
    step_timer->setOverflow(5000, HERTZ_FORMAT);
    step_timer->setMode(1, TIMER_OUTPUT_COMPARE_PW1, step_pin);
    step_timer->setCaptureCompare(1, 50, PERCENT_COMPARE_FORMAT);
    step_timer->pause();

}

void SC_Motor::set_speed(uint16_t pulsefreq){
    step_timer->setOverflow(5000, HERTZ_FORMAT);
    step_timer->refresh();
}

void SC_Motor::set_direction(bool clockwise){
    digitalWrite(dir_pin, clockwise);
}

void SC_Motor::start(){
    digitalWrite(nEnable_pin, LOW);
    step_timer->resume();
}

void SC_Motor::stop(){
    step_timer->pause();
    digitalWrite(nEnable_pin, HIGH);
}

void SC_Motor::move_n_pulses(uint32_t pulses){
    
}