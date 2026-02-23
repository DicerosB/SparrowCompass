#include "sc_motor.h"


SC_Motor::SC_Motor(uint8_t nEnable_pin, uint8_t step_pin, uint8_t dir_pin, uint8_t sync_pin){
    
    //GPIO config
    this->nEnable_pin = nEnable_pin;
    this->step_pin = step_pin;
    this->dir_pin = dir_pin;
    this->sync_pin = sync_pin;

    pinMode(nEnable_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(sync_pin, INPUT)

    digitalWrite(nEnable_pin, HIGH);
    digitalWrite(dir_pin, LOW);
    // Member init
    target_speed = 5000; 
    rot_direction = CW;
    currently_moving = false;
    synchronized = false;
    // Hardware Timer
    TIM_TypeDef *Instance = TIM10;
    step_timer = new HardwareTimer(Instance);
    step_timer->setOverflow(5000, HERTZ_FORMAT);
    step_timer->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, step_pin);
    step_timer->setCaptureCompare(1, 50, PERCENT_COMPARE_FORMAT);
    step_timer->attachInterrupt(pwm_callback); 
    synchronize();
}

void SC_Motor::set_speed(uint16_t rpm){
    target_speed = rpm;
}
uint16_t get_speed(){
    return target_speed;
}
uint16_t get_current_speed(){
    return freq2rpm(step_timer->setOverflow(HERTZ_FORMAT));
}

void SC_Motor::set_direction_cw(){
    rot_direction = CW
}

void SC_Motor::set_direction_ccw(){
    rot_direction = CCW
}

bool SC_Motor::get_direction(){
    return direction;
}

bool SC_Motor::is_moving(){
    return currently_moving;
}

void SC_Motor::move_inf(){
    // start moving motor forever
    digitalWrite(dir_pin, (uint8_t)rot_direction+1);
    digitalWrite(nEnable_pin, LOW);
    currently_moving = true;
    step_timer->setOverflow(rpm2freq(speed), HERTZ_FORMAT);
    step_timer->resume();
}

void SC_Motor::stop(){
    // stop any movement
    step_timer->pause();
    currently_moving = false;
    digitalWrite(nEnable_pin, HIGH);
}

void SC_Motor::synchronize(){
    // move motor slowly a full rotation until synchronized
    uint16_t current_speed = target_speed;
    target_speed = SYNC_SPEED;
    move_n_pulses(USTEPS_PER_REVOLUTION);
    while(currently_moving){
        if(synchronized){
            stop();
            target_speed = current_speed;
            return;
        }
    }
    target_speed = current_speed;
}

uint8_t SC_Motor::move_n_pulses(uint32_t pulses){
    target_counter = pulse_counter + (pulses * rot_direction);
    digitalWrite(dir_pin, (uint8_t)rot_direction+1);
    digitalWrite(nEnable_pin, LOW);
    currently_moving = true;
    step_timer->resume();
}

uint8_t SC_Motor::move_to_heading(uint32_t heading_mdegrees, uint8_t mode){
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
        pulse_counter = USTEPS_PER_REVOLUTION;
    }else if(pulse_counter == USTEPS_PER_REVOULTION && rot_direction == CW){
        pulse_counter = 0;
    }else{
        pulse_counter += rot_direction; 
    }

    int delta_pulses = abs(target_counter - pulse_counter);
    // terminate movement if target reached
    if(delta_pulses == 0){
        step_timer->pause();
        digitalWrite(nEnable_pin, HIGH);
        current_speed = 0;
        currently_moving = false;
    }
    // adjust speed if needed
    // check if deceleration is necessary
    uint16_t current_speed = freq2rpm(step_timer->setOverflow(HERTZ_FORMAT))
    uint8_t acc_steps_pending = (delta_pulses / USTEPS_PER_ACC_STEP); // acceleration steps left until speed must be 0;
    uint16_t max_speed = ACCELERATION * (acc_steps_pending+1); //max allowed speed for this acc step
    if(current_speed > max_speed){
        step_timer->setOverflow(rpm2freq(current_speed), HERTZ_FORMAT);
        return;
    }
    //check if acceleration is applicable
    if(cu)
    

}

uint16_t rpm2freq(uint16_t rpm){
    return (uint16_t)((rpm * USTEPS_PER_REVOLUTION) / 60)
}
uint16_t freq2rpm(uint32_t freq){
    return (uint16_t)((freq * 60) / USTEPS_PER_REVOLUTION)
}