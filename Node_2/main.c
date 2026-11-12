#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "time.h"
#include "can.h"
#include "joystick.h"
#include "servo.h"
#include "adc.h"
#include "encoder.h"
#include "motor.h"
#include "solenoid.h"
#include "ir.h"

#define F_CPU 84000000

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */


int main()
{
    SystemInit();
    PMC->PMC_PCER0 = 1u << ID_PIOB; // enable clock

    
    


    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    /*
    PIOB->PIO_PER = PIO_PER_P13;
    PIOB->PIO_OER = PIO_OER_P13;
    PIOB->PIO_SODR = PIO_SODR_P13;
  */

    //Uncomment after including uart above
    uart_init(F_CPU, 9600);
    

    can_init((CanInit){.brp = 20, .sjw = 0, .phase1 = 2, .phase2 = 2, .propag = 0, .smp = 0}, 0);
    servo_init();
    adc_init();
    encoder_init();
    motor_init();
    solenoid_init();
    //time_spinFor(msecs(2000));

    int32_t current_pulse_width = 1500;
    int32_t max_increment = 100;
    Joystick_Calibration calibration_x = {.max_pos = 248, .min_pos = 70, .neutral_buffer = 5, .neutral = 166};
    Joystick_Calibration calibration_y = {.max_pos = 247, .min_pos = 68, .neutral_buffer = 5, .neutral = 165};
    Joystick_Position joystick_pos = joystick_read_position();
    Encoder_Data encoder_data = encoder_get_data();
    int32_t ref = 0;
    int32_t integral_value = 0;
    int32_t servo_PWM_val = 1500;
    int32_t old_joystick_y_pos = 165;
    int32_t active = 0;


    while (1){
        
        joystick_pos = joystick_read_position();

        if (joystick_pos.y_pos >= 160 &&
            joystick_pos.y_pos <= 170)
        {
            old_joystick_y_pos = 165;
        }
        else if (joystick_pos.y_pos != 0)
        {
            old_joystick_y_pos = joystick_pos.y_pos;
        }
                
        servo_PWM_val = 1100 + ((old_joystick_y_pos - 68) * 800) / 179;
            
        
        solenoid_pulse(&active, joystick_pos.r6_pressed);

        //printf("xpos: %i\r\n", measured_pos);
        //time_spinFor(msecs(1));
        
        servo_update_pulse_width(servo_pulse_width_guard(servo_PWM_val));
        
        //servo_move(joystick_pos.y_pos, &current_pulse_width, max_increment, calibration_y);
        motor_update_ref(&ref, joystick_pos.x_pos, max_increment, calibration_x);
        encoder_data = encoder_get_data();
        motor_PI_control(encoder_data.pos, ref, &integral_value, 0.05f, 0.0f);

        //motor_update_pulse_width(20);
        //printf("adc: %lu\r\n", adc_read());
        //encoder_print_status();
        

        ir_send_ball_lost();
        time_spinFor(msecs(1));
        
        //motor_control_by_joystick(measured_pos.x_pos);
    }
}