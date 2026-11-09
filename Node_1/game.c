#include "game.h"
#include "oled.h"
#include "position.h"
#include <stdint.h>



void game_menu_update(Position_Direction dir, MenuOption* menuOption){
    if(dir != 0){
        *menuOption = oled_menu_switch(dir, *menuOption);
        oled_main_menu(*menuOption);
    }
    
    
}
void game_start(MenuOption menuOption,
                Game_State *game_state,
                uint8_t *lives)
{
    switch (menuOption)
    {
        case EASY_MODE:
            *lives = 3;
            break;

        case NORMAL_MODE:
            *lives = 2;
            break;

        case HARD_MODE:
            *lives = 1;
            break;
    }

    *game_state = PLAYING;

    oled_game_screen(*lives);
}

void game_lose_life(Game_State *game_state, uint8_t *lives)
{
    if (*lives > 0)
    {
        (*lives)--;
    }

    if (*lives == 0)
    {
        *game_state = GAME_OVER;
        oled_game_over();
    }
    else
    {
        oled_game_screen(*lives);
    }
}

void game_back_to_menu(Game_State *game_state,
                       MenuOption menuOption)
{
    *game_state = MENU;
    oled_main_menu(menuOption);
}
