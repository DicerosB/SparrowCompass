#include "sc_motor.h"

SC_Motor::SC_Motor(uint8_t nEnable_pin, uint8_t step_pin, uint8_t dir_pin){
    this->nEnable_pin = nEnable_pin;
    this->step_pin = step_pin;
    this->dir_pin = dir_pin;
    this->hw_timer = new HardwareTimer((TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(step_pin), PinMap_PWM));
    this->timer_channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(step_pin), PinMap_PWM));
    this->is_moving = 0;
    this->pulse_counter = 0;
    hw_timer->setOverflow(1000, HERTZ_FORMAT);
    hw_timer->setMode(timer_channel, TIMER_OUTPUT_COMPARE_PWM1, step_pin);
    hw_timer->setCaptureCompare(timer_channel, 50, PERCENT_COMPARE_FORMAT);


    //configure pins
    pinMode(nEnable_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);

    digitalWrite(nEnable_pin, HIGH);
    digitalWrite(step_pin, LOW);
    digitalWrite(dir_pin, LOW);
}

void SC_Motor::set_speed(uint16_t pulsefreq){
    hw_timer->setOverflow(pulsefreq, HERTZ_FORMAT);
}

void SC_Motor::set_direction(bool clockwise){
    digitalWrite(dir_pin, clockwise);
}

void SC_Motor::start(){
    digitalWrite(nEnable_pin, LOW);
    hw_timer->resume();
    is_moving = 1;
}

void SC_Motor::stop(){
    digitalWrite(nEnable_pin, HIGH);
    hw_timer->pause();
    if(hw_timer->hasInterrupt()){
        hw_timer->detachInterrupt();
        hw_timer->refresh();
    }
    is_moving = 0;
}

void SC_Motor::move_n_pulses(uint32_t pulses){
    pulse_counter = pulses;
    hw_timer->attachInterrupt(timer_channel, std::bind(timer_interrupt_callback, this));
    hw_timer->refresh();
    is_moving = 1;
    hw_timer->resume();


}

void timer_interrupt_callback(SC_Motor* instance){
    instance->pulse_counter--;
    if(instance->pulse_counter <= 0){
        instance->stop();
    }
}