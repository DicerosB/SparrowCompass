#include "sc_motor.h"

SC_Motor::SC_Motor(uint8_t nEnable_pin, uint8_t step_pin, uint8_t dir_pin){
    this->nEnable_pin = nEnable_pin;
    this->step_pin = step_pin;
    this->dir_pin = dir_pin;
    pinMode(nEnable_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);

    digitalWrite(nEnable_pin, HIGH);
    digitalWrite(step_pin, LOW);
    digitalWrite(dir_pin, LOW);
}

void SC_Motor::set_speed(uint16_t pulsefreq){

}

void SC_Motor::set_direction(bool clockwise){
    digitalWrite(dir_pin, clockwise);
}

void SC_Motor::start(){
    digitalWrite(nEnable_pin, LOW);
}

void SC_Motor::stop(){
    digitalWrite(nEnable_pin, HIGH);
}

void SC_Motor::move_n_pulses(uint32_t pulses){

}