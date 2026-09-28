#include <avr/io.h>
#include "oled.h"
#include "spi.h"
#include "fonts.h"
#include <avr/pgmspace.h>
#include <stdbool.h>

#define DC_DISPLAY PB1

#define OLED_CMD_PAGE_START 0xB0
#define OLED_CMD_COLUMN_LOW_START  0x00
#define OLED_CMD_COLUMN_HIGH_START 0x10
#define OLED_CMD_DISPLAY_OFF        0xAE
#define OLED_CMD_DISPLAY_ON         0xAF
#define OLED_CMD_ADDRESS_MODE       0x20
#define OLED_ADDRESS_MODE_PAGE      0x02
#define OLED_CMD_NORMAL_DISPLAY     0xA6
#define OLED_CMD_DISPLAY_FROM_RAM   0xA4
#define OLED_WIDTH 128
#define OlED_FLIP 0xA1
#define OLED_TEXT 0xC8

void oled_init(void){
    DDRB |= (1 << DC_DISPLAY); //output
    PORTB &= ~(1 << DC_DISPLAY); // start lav

    oled_command(OLED_CMD_DISPLAY_OFF); //display av under konfig

    oled_command(OlED_FLIP);
    oled_command(OLED_TEXT);

    oled_command(OLED_CMD_ADDRESS_MODE); //set minne adressering
    oled_command(OLED_ADDRESS_MODE_PAGE); //set page adressering


    oled_command(OLED_CMD_DISPLAY_FROM_RAM); //innhold fra ram
    oled_command(OLED_CMD_NORMAL_DISPLAY); // Ram-bit 1 = pixel er på
    oled_command(OLED_CMD_DISPLAY_ON); // skru på
}

void oled_command(uint8_t command){
    PORTB &= ~(1 << DC_DISPLAY); //sender kommando

    spi_select_slave(SPI_SLAVE_DISPLAY); // velge riktig slave
    spi_write(command); // commanden sendes
    spi_deselect_all();
}

void oled_write(uint8_t data){
    PORTB |= (1 << DC_DISPLAY); // sender data

    spi_select_slave(SPI_SLAVE_DISPLAY);
    spi_write(data);
    spi_deselect_all();
}

void oled_goto_line(OLED_Page page){
    if(page > OLED_PAGE_7){
        return;
    }
    oled_command(OLED_CMD_PAGE_START | page);

}

void oled_goto_column(uint8_t column){
    if(column > 127){
        return;
    }
    oled_command(OLED_CMD_COLUMN_LOW_START | (column & 0x0F));
    oled_command(OLED_CMD_COLUMN_HIGH_START | (column >> 4));
}

void oled_pos(OLED_Page page, uint8_t column){
    oled_goto_line(page);
    oled_goto_column(column);
}

void oled_print_char(char c){
    if(c < 32 || c > 126){
        return;
    }

    uint8_t index = c - 32;

    for (uint8_t i = 0; i < 5; i++){
        uint8_t column = pgm_read_byte(&font5[index][i]);
        oled_write(column);
    }
    oled_write(0x00); // mellomrom
}

void oled_print(const char *text){
    while(*text != '\0'){
        oled_print_char(*text);
        text += 1;
    }
}

void oled_home(){
    oled_pos(OLED_PAGE_0, 0);
}


void oled_clear_line(OLED_Page page){
    if(page > OLED_PAGE_7){
        return;
    }
    oled_pos(page, 0);
    for(uint8_t column = 0; column < OLED_WIDTH; column ++){
        oled_write(0x00);
    }
}

void oled_clear(void){
    for(OLED_Page page = OLED_PAGE_0; page <= OLED_PAGE_7; page++){
        oled_clear_line(page);
        }
    
    oled_home();

}

/*
void oled_main_menu(void){
    oled_clear();
    oled_pos(OLED_PAGE_0, 10);
    oled_print("1. alternative");
    oled_pos(OLED_PAGE_2, 10);
    oled_print("2. alternative");

    oled_pos(OLED_PAGE_4, 10);
    oled_print("3. alternative");

    oled_pos(OLED_PAGE_6, 10);
    oled_print("4. alternative");
    
}
    */

void oled_print_menu_item(OLED_Page page, const char *text, bool selected){
    uint8_t kolonner = 0;

    oled_pos(page, 0);
    for(uint8_t i = 0; i < 10; i ++){
        oled_write(selected ? 0xFF : 0x00);
        kolonner ++;

    }

    
    while (*text != '\0') {
        char c = *text;
        

        if (c >= 32 && c <= 126) {
            uint8_t index = c - 32;

            for (uint8_t i = 0; i < 5; i++) {
                uint8_t data = pgm_read_byte(&font5[index][i]);

                if (selected) {
                    data = ~data;
                }
                

                oled_write(data);
                kolonner ++;
            }

            if(kolonner < 128){
            oled_write(selected ? 0xFF : 0x00);
            kolonner ++;

            }
        }

        text++;
    }
    while (kolonner < 128){

        oled_write(selected ? 0xFF : 0x00);
        kolonner++;
    }
    
    


}



void oled_main_menu(uint8_t selected)
{
    oled_clear();

    oled_print_menu_item(OLED_PAGE_0, "Easy Mode", selected == 0);
    oled_print_menu_item(OLED_PAGE_2, "Normal Mode", selected == 1);
    oled_print_menu_item(OLED_PAGE_4, "Hard Mode", selected == 2);
    oled_print_menu_item(OLED_PAGE_6, "Impossible", selected == 3);
}

const char *menu_option_to_string(MenuOption option){
    switch (option) {
        case EASY_MODE:
            return "Easy mode";

        case NORMAL_MODE:
            return "Normal mode";

        case HARD_MODE:
            return "Hard mode";

        case IMPOSIBLE:
            return "Impossible";

        default:
            return "Unknown";
    }
}
