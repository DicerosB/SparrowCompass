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
    this->step_timer->setOverflow(5000, HERTZ_FORMAT);
    this->step_timer->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, step_pin);
    this->step_timer->setCaptureCompare(1, 50, PERCENT_COMPARE_FORMAT);
    digitalWrite(nEnable_pin, LOW);
    this->start();
    delay(2000);
    Serial.println("Stop Motor.\n");
    this->stop();
    Serial.println("Stopped Motor.\n");
    this->set_direction(!this->get_direction());
    delay(2000);
    Serial.println("Start Motor.\n");
    this->start();
    Serial.println("Started Motor.\n");
    delay(2000);
    this->stop();

}

void SC_Motor::set_speed(uint16_t pulsefreq){
    this->step_timer->setOverflow(5000, HERTZ_FORMAT);
    this->step_timer->refresh();
}

void SC_Motor::set_direction(bool clockwise){
    digitalWrite(dir_pin, clockwise);
}

bool SC_Motor::get_direction(){
    return this->direction;
}

void SC_Motor::start(){
    digitalWrite(nEnable_pin, LOW);
    this->step_timer->resume();
}

void SC_Motor::stop(){
    this->step_timer->pause();
    digitalWrite(nEnable_pin, HIGH);
}

void SC_Motor::move_n_pulses(uint32_t pulses){

}