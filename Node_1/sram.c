#include <avr/io.h>
#include "sram.h"

void sram_init(){
    MCUCR |= (1 << SRE);

}