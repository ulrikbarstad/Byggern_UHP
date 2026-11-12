#include <stdint.h>
#include <stdio.h>
#include "motor.h"
#include "sam.h"
#include "joystick.h"

void motor_init(){

    PIOB->PIO_PDR |= PIO_PB12; // Disable IO mode for PB13
    PIOB->PIO_ABSR |= PIO_PB12; //Enables peripheraø function for PB13 -> PWM mode
    PIOC->PIO_PER |= PIO_PC23; //Enable PC23
    PIOC->PIO_OER |= PIO_PC23; //Enable output for PC23
    PIOC->PIO_SODR = PIO_PC23; // set direction to 1

    PMC->PMC_WPMR = 0x504D4300; // Disables write protect mode register

    //CLK A = 1*10⁶ from servo_init()

    REG_PWM_CMR0 &= ~(PWM_CMR_CALG); //Sets CALG to 0 so that PWM is left aligned
    REG_PWM_CMR0 |= (PWM_CMR_CPOL); // Starts with high level
    REG_PWM_CMR0 |= 0b1011; // Selects clock A for channel 0

    REG_PWM_CPRD0 = 50; // Sets channel period to be 50 ticks of clock A, so that PWM freq = 20 kHz
    REG_PWM_CDTY0 = 0; // Set pulse width to 0 so that motor is still
    PWM->PWM_ENA |= PWM_SR_CHID0; // enable channel id 0
}
    
void motor_update_pulse_width(int32_t pulse_width){ //This updates pulse width in PWM register
    if (pulse_width > 50){pulse_width = 50;}
    if(pulse_width < 0){pulse_width = 0;}
    REG_PWM_CDTYUPD0 = pulse_width;

}

void motor_set_direction(int32_t dir){
    if (dir == 0){PIOC->PIO_CODR = PIO_PC23;}
    else if (dir == 1) {PIOC->PIO_SODR = PIO_PC23;}
}

int32_t motor_get_increment(int32_t x_pos, int32_t max_increment, Joystick_Calibration calibration){
    joystick_scale_position(&x_pos, calibration);
    int32_t increment = (max_increment * x_pos) / 100;
    return increment;
}

void motor_update_ref(int32_t* ref, int32_t x_pos, int32_t max_increment, Joystick_Calibration calibration){
    int32_t new_ref = *ref - motor_get_increment(x_pos, max_increment, calibration);
    if (new_ref > 2810){*ref = 2810;}
    else if(new_ref < 0){*ref = 0;}
    else{*ref = new_ref;}
}

void motor_PI_control(int32_t pos, int32_t ref, int32_t* integral_value, _Float32 k_p, _Float32 k_i){
    int32_t error = pos - ref;

    // Deadzone around reference
    if (error > -30 && error < 30) {
        *integral_value = 0;
        motor_update_pulse_width(0);
        return;
    }

    // Integrate error
    *integral_value += error;

    // Anti-windup
    if (*integral_value > 100000) {
        *integral_value = 100000;
    }
    else if (*integral_value < -100000) {
        *integral_value = -100000;
    }

    float u = k_p * error +
              k_i * (*integral_value);

    // Sign decides direction
    if (u > 0) {
        motor_set_direction(1);
    }
    else {
        motor_set_direction(0);
        u = -u;
    }

    u += 1;


    if (u > 50) {u = 50;} //Øvre tak

    

    motor_update_pulse_width((int32_t)u);


    
/*
        if (error == 0){motor_update_pulse_width(0);}
    else{
        if (error > 0){motor_set_direction(1);}
        else{motor_set_direction(0);}
        int32_t u = (int32_t)(k_p * abs(error) + k_i*(abs(*integral_value)));
        if(u > 0 && u < 15){u = 15;}
        motor_update_pulse_width(u);
        if (pos > 2800 || pos < 10){*integral_value = 0;}
        else{*integral_value += error;}
    } 
    if (i % 100000 == 0){
        printf("error: %i \r\npos: %i\r\n", error, pos);}*/
}



void motor_control_by_joystick(int32_t x_pos){
   
    if (x_pos > 180){
        motor_set_direction(1);
        motor_update_pulse_width(15);}
    else if(x_pos < 150){
        motor_set_direction(0);
        motor_update_pulse_width(15);
    }
    else{
        motor_update_pulse_width(0);
    }
}

