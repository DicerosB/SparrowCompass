#include "sc_motor.h"


SC_Motor::SC_Motor(uint8_t nEnable_pin, uint8_t step_pin, uint8_t dir_pin, uint8_t sync_pin, USBSerial* p_usb):
    nEnable_pin(nEnable_pin),
    step_pin(step_pin),
    dir_pin(dir_pin),
    sync_pin(sync_pin),
    usb(p_usb),
    pulse_counter(0),
    rot_direction(CW),
    currently_moving(false),
    moving_infinitely(false),
    synchronized(false)
{
    
    //GPIO config
    pinMode(nEnable_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(sync_pin, INPUT);

    digitalWrite(nEnable_pin, HIGH);
    digitalWrite(dir_pin, LOW);
    // Timer Init
    step_timer = new HardwareTimer(TIM10);
    step_timer->setOverflow((MAX_ACCELERATION_PER_EVAL_T_Q8>>8), HERTZ_FORMAT);   // initial frequency
    step_timer->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, step_pin);    // ouput pin
    step_timer->setCaptureCompare(1, 50, PERCENT_COMPARE_FORMAT);   // 50% duty cycle
    step_timer->attachInterrupt(step_callback_helper);
    step_timer->pause();

    speed_timer = new HardwareTimer(TIM11);
    speed_timer->setOverflow(SPEED_EVAL_FREQ, HERTZ_FORMAT);
    speed_timer->attachInterrupt(speed_callback_helper);
    speed_timer->pause();
    //synchronize();
}

uint16_t SC_Motor::get_heading(){
    return (pulse_counter*HEADING_FRACTIONS_PER_MICROSTEP_Q9) >> 9;
}

bool SC_Motor::is_moving(){
    return currently_moving;
}

void SC_Motor::move(uint8_t control, uint8_t speed, uint16_t heading){
    /*
    *   Starts movement of the motor. Movement is acelleration controlled.
    *   Parameters:
    *   control     : flags that determine movement mode
    *                   Bit 7: direction (0: CW, 1:CCW)
    *                   Bit 6: move infinitely
    *                   Bit 5: use fastest way to heading (ignore dir flag)
    *   speed       : speed of movement (in rpm)
    *   heading     : heading, 0..65535 is mapped to one rotation
    */

    if(!speed) return; // speed must not be 0.

    // calculate target counter
    target_counter = (heading<<9)/HEADING_FRACTIONS_PER_MICROSTEP_Q9;
    target_speed = speed;
    rot_direction = control >> 7;
    if((control&0x20)>>5){
        //calculate rot_direction for shortest movement
        rot_direction = (uint16_t)(target_counter - pulse_counter) > 0xEFFF;
    }
    digitalWrite(dir_pin, rot_direction); // set dir pin
    digitalWrite(nEnable_pin, LOW); // enable motor
    currently_moving = true;
    moving_infinitely = (control&0x20)>>5;
    step_timer->resume();   // continue PWM generation
    speed_timer->resume();
}

void SC_Motor::stop(){
    // stop as fast as possible
    if(!currently_moving) return;
    // count eval periods needed for full stop
    uint16_t speed = step_timer->getOverflow(HERTZ_FORMAT);
    uint16_t v = 0;
    uint16_t d = 0;
    uint16_t periods= 0;
    while(v <= speed){
        periods++;
        v = (periods * MAX_ACCELERATION_PER_EVAL_T_Q8) >> 8; // Steps/s
        d += v / SPEED_EVAL_FREQ;
    }
    target_counter = d + 10; // 10 is just some safety margin (TODO!)
}

uint8_t SC_Motor::synchronize(){
    
}

void SC_Motor::sync_callback(){
    synchronized = true;
    pulse_counter = 0;
}

void SC_Motor::step_timer_period_callback(){
    /*
        gets called each pulse
        keeps track of current rotational position,
    */
    // catch over/ underflow
    if(!rot_direction && pulse_counter == USTEPS_PER_REVOLUTION-1){
        pulse_counter = 0;
    }
    else if (rot_direction && pulse_counter == 0)
    {
        pulse_counter = USTEPS_PER_REVOLUTION-1;
    }
    else{
        pulse_counter += 1 - (2* rot_direction); // increment / decrement according to rot_direction
    }

    // stop if target reached
    if(!moving_infinitely && pulse_counter == target_counter){
        step_timer->pause();
        speed_timer->pause();
        *usb << "target reached at " << pulse_counter <<".\n";
        digitalWrite(nEnable_pin, HIGH);
        currently_moving = false;
    }

}

void SC_Motor::speed_timer_period_callback(){
    /*
    *   gets called peroidically
    *   controls maximum ac-/ deceleration
    */
    digitalWrite(DEBUG_LED_Pin, HIGH);

    //calculate rotational distance to target
    uint16_t distance = 0;
    if(!rot_direction){
        // clockwise rotation
        distance = target_counter - pulse_counter;
    }else{
        distance = pulse_counter - target_counter;
    }
    
    uint16_t speed = step_timer->getOverflow(HERTZ_FORMAT); // steps per second
    *usb << distance << " : " << speed << " ";
    // determine if breaking is needed

    // count periods needed for full stop
    uint16_t v = 0;
    uint16_t d = 0;
    uint16_t periods= 0;
    while(v <= speed){
        periods++;
        v = (periods * MAX_ACCELERATION_PER_EVAL_T_Q8) >> 8; // Steps/s
        d += v / SPEED_EVAL_FREQ;
        if( d > distance){
            // breaking condition
            *usb << "b\n";
            step_timer->setOverflow(speed - (MAX_ACCELERATION_PER_EVAL_T_Q8>>8), HERTZ_FORMAT);
            step_timer->refresh();
            digitalWrite(DEBUG_LED_Pin, LOW);
            return;
        }
    }
    
    // determine if acceleration is allowed
    uint16_t target_f = (target_speed * RPM_CONVERSION_FACTOR_Q8) >> 8;
    if(speed < target_f){
        speed += MAX_ACCELERATION_PER_EVAL_T_Q8 >> 8;
        if(speed > target_speed)speed = target_f;
        step_timer->setOverflow(speed, HERTZ_FORMAT);
        *usb << "a ";
    }
    *usb << "\n";
    digitalWrite(DEBUG_LED_Pin, LOW);
    
}

void step_callback_helper(){
    if(motor && motor->step_timer){ //temporary fix for prefiring interrupt
        motor->step_timer_period_callback();
    }
}

void speed_callback_helper(){
    if(motor && motor->speed_timer){ //temporary fix for prefiring interrupt
        motor->speed_timer_period_callback();
    }
}