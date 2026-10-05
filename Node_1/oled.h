#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    EASY_MODE = 0,
    NORMAL_MODE,
    HARD_MODE,
    IMPOSSIBLE
} MenuOption;

typedef enum {
    OLED_PAGE_0 = 0,
    OLED_PAGE_1,
    OLED_PAGE_2,
    OLED_PAGE_3,
    OLED_PAGE_4,
    OLED_PAGE_5,
    OLED_PAGE_6,
    OLED_PAGE_7
} OLED_Page;

void oled_init(void);
void oled_command(uint8_t command);
void oled_write(uint8_t data);
void oled_goto_line(OLED_Page page);
void oled_goto_column(uint8_t column);
void oled_pos(OLED_Page page, uint8_t column);
void oled_print_char(char c);
void oled_print(const char *text);
void oled_clear(void);
void oled_home(void);
void oled_clear_line(OLED_Page page);
void oled_main_menu(uint8_t selected);
void oled_print_menu_item(OLED_Page page, const char *text, bool selected);
const char *menu_option_to_string(MenuOption option);
void oled_menu_switch(Position_Direction dir);

#endif