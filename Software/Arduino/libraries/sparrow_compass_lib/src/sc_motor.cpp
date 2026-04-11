#include "sc_motor.h"


SC_Motor::SC_Motor(uint8_t nEnable_pin, uint8_t step_pin, uint8_t dir_pin, uint8_t sync_pin){
    
    //GPIO config
    this->nEnable_pin = nEnable_pin;
    this->step_pin = step_pin;
    this->dir_pin = dir_pin;
    this->sync_pin = sync_pin;

    pinMode(nEnable_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(sync_pin, INPUT);

    digitalWrite(nEnable_pin, HIGH);
    digitalWrite(dir_pin, LOW);
    // Member init
    target_speed = 5000; 
    rot_direction = CW;
    rot_counter = 0;
    currently_moving = false;
    moving_infinitely = false;
    synchronized = false;
    // Hardware Timer
    // TIM_TypeDef *Instance = TIM10;
    step_timer = new HardwareTimer(TIM10);
    step_timer->setOverflow(5000, HERTZ_FORMAT);
    step_timer->getOverflow();
    step_timer->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, step_pin);
    step_timer->setCaptureCompare(1, 50, PERCENT_COMPARE_FORMAT);
    step_timer->attachInterrupt(callback_helper); 
    //synchronize();
}

void SC_Motor::set_speed(uint16_t rpm){
    target_speed = (rpm>MAX_SPEED)? MAX_SPEED : rpm;
}
uint16_t SC_Motor::get_speed(){
    return target_speed;
}
uint16_t SC_Motor::get_current_speed(){
    return freq2rpm(step_timer->getOverflow(HERTZ_FORMAT));
}

void SC_Motor::set_direction_cw(){
    rot_direction = CW;
}

void SC_Motor::set_direction_ccw(){
    rot_direction = CCW;
}

bool SC_Motor::get_direction(){
    return rot_direction;
}

bool SC_Motor::is_moving(){
    return currently_moving;
}

void SC_Motor::move_inf(){
    // start moving motor forever
    digitalWrite(dir_pin, (uint8_t)rot_direction+1);
    digitalWrite(nEnable_pin, LOW);
    currently_moving = true;
    moving_infinitely = false;
    step_timer->setOverflow(rpm2freq(1), HERTZ_FORMAT);
    step_timer->resume();
}

void SC_Motor::stop(){
    // stop as fast as possible
    moving_infinitely = false;
    uint32_t pulse_delta = uint16_t(get_current_speed()/ACCELERATION) + 2; // + 2 ensures reaching min velocity in time;
    target_counter = pulse_counter + (pulse_delta * rot_direction);
}

uint8_t SC_Motor::synchronize(){
    // move motor slowly a full rotation until synchronized, this function is blocking
    uint16_t pevious_speed = target_speed;
    target_speed = SYNC_SPEED;
    rot_counter = 0;
    move_n_pulses(USTEPS_PER_REVOLUTION);
    while(currently_moving){
        if(synchronized){
            stop();
            target_speed = pevious_speed;
            return true;
        }
    }
    target_speed = pevious_speed;
    return false;
}

void SC_Motor::move_n_pulses(uint32_t pulses){
    target_counter = pulse_counter + (pulses * rot_direction);
    digitalWrite(dir_pin, (uint8_t)rot_direction+1);
    digitalWrite(nEnable_pin, LOW);
    currently_moving = true;
    moving_infinitely = false;
    step_timer->resume();
}

void SC_Motor::move_to_heading(uint32_t heading_mdegrees, uint8_t mode){
    /* 
        move motor to a specific heading
        modes: 
            mode & (0x01)? force current rot_direction : use shortest rot_direction
            mode & (0x02)? smooth acc-/deceleration: fast mode
    */
}
void SC_Motor::sync_callback(){
    synchronized = true;
    pulse_counter = 0;
}

void SC_Motor::timer_period_callback(){
    /*
        gets called each pulse
        keeps track of current heading and speed,
    */

    // increase /decrease pulse counter
    if(pulse_counter == 0 && rot_direction == CCW){
        //underflow
        pulse_counter = USTEPS_PER_REVOLUTION;
        rot_counter--;
    }else if(pulse_counter == USTEPS_PER_REVOLUTION && rot_direction == CW){
        //overflow
        pulse_counter = 0;
        rot_counter++;
    }else{
        pulse_counter += rot_direction; 
    }

    if(moving_infinitely) return;

    int delta_pulses = abs(target_counter - pulse_counter);
    // terminate movement if target reached
    if(delta_pulses == 0){
        step_timer->pause();
        digitalWrite(nEnable_pin, HIGH);
        currently_moving = false;
    }

    // Speed Control    
    uint16_t current_speed = get_current_speed();
    // check max allowed speed for this delta
    uint16_t max_speed = (delta_pulses * ACCELERATION) + 1;
    if(current_speed > max_speed){
        //decelerate
        uint16_t new_speed=(current_speed>ACCELERATION)? current_speed-ACCELERATION : 1;
        step_timer->setOverflow(rpm2freq(new_speed));
        return;
    } 
    if(current_speed < target_speed){
        //accelerate
        uint16_t new_speed=(current_speed+ACCELERATION>target_speed)? target_speed : current_speed+ACCELERATION;
        step_timer->setOverflow(rpm2freq(new_speed));
        return;
    }

}

uint16_t SC_Motor::rpm2freq(uint16_t rpm){
    return (uint16_t)((rpm * USTEPS_PER_REVOLUTION) / 60);
}
uint16_t SC_Motor::freq2rpm(uint32_t freq){
    return (uint16_t)((freq * 60) / USTEPS_PER_REVOLUTION);
}

void callback_helper(){
    if(motor && motor->step_timer){ //temporary fix for prefiring interrupt
        motor->timer_period_callback();
    }
}