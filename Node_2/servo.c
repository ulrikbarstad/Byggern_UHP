#include <stdint.h>
#include "servo.h"
#include "sam.h"
#include "joystick.h"

void servo_init(){

    PIOB->PIO_PDR |= PIO_PB13; // Disable IO mode for PB13
    PIOB->PIO_ABSR |= PIO_PB13; //Enables peripheraø function for PB13 -> PWM mode

    PMC->PMC_WPMR = 0x504D4300; // Disables write protect mode register

    PMC->PMC_PCER1 |= (1 << 4); // Enables peripheral clock for PWM
    PWM->PWM_CLK = 0; // set PREA (prescaler for clock A) to 1 so that CLK = MCK/DIV_A = 84*10⁶ / DIV_A
    PWM->PWM_CLK |= 84; // set Div A to 84 so that CLK A = 1*10⁶

    REG_PWM_CMR1 &= ~(PWM_CMR_CALG); //Sets CALG to 0 so that PWM is left aligned
    REG_PWM_CMR1 |= (PWM_CMR_CPOL); // Starts with high level
    REG_PWM_CMR1 |= 0b1011; // Selects clock A for channel 1

    REG_PWM_CPRD1 = 20000; // Sets channel period to be 20000 ticks of clock A, so that PWM period = 10⁻6 * 20 000 = 20 ms
    REG_PWM_CDTY1 = 1500; // Set pulse width to 1.5 ms = center position
    PWM->PWM_ENA |= PWM_SR_CHID1; // enable channel id 1
}

int32_t servo_pulse_width_guard(int32_t pulse_width){ //this is a guard which controls that the pulse width will be between 900 and 2100
    if (pulse_width > 2100){
        return 2100;
    }
    else if (pulse_width < 900){
        return 900;
    }
    return pulse_width;
}

int32_t servo_get_increment(int32_t max_increment, Joystick_X_Calibration calibration){
    int32_t x_pos = joystick_read_x_position();
    joystick_scale_x_position(&x_pos, calibration);
    int32_t increment = (max_increment * x_pos) / 100;
    return increment;
}


void servo_update_pulse_width(int32_t pulse_width){ //This updates pulse width in PWM register
    REG_PWM_CDTY1 = pulse_width;
}

void servo_move(int32_t* current_pulse_width, int32_t max_increment, Joystick_X_Calibration calibration){
    int32_t increment = servo_get_increment(max_increment, calibration);
    *current_pulse_width = servo_pulse_width_guard(*current_pulse_width - increment);
    printf("increment %i \r\n\r\n pw %i \r\n\r\n" , increment , *current_pulse_width);
    servo_update_pulse_width(*current_pulse_width);
}



