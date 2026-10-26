#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "time.h"
#include "can.h"
#include "joystick.h"
#include "servo.h"
#include "adc.h"

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
    //time_spinFor(msecs(2000));

    int32_t current_pulse_width = 1500;
    int32_t max_increment = 60;
    Joystick_X_Calibration calibration = {.max_x_pos = 247, .min_x_pos = 68, .neutral_buffer = 5, .neutral_x = 165};

    while (1){
        time_spinFor(msecs(50));
        servo_move(&current_pulse_width, max_increment, calibration);


        printf("adc: %lu\r\n", adc_read());
    }
    
}