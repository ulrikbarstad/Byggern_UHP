#ifndef GAME
#define GAME
#include <stdint.h>
#include "position.h"


typedef enum {
    MENU,
    PLAYING,
    GAME_OVER
} Game_State;


typedef enum {
    EASY_MODE = 0,
    NORMAL_MODE,
    HARD_MODE
} MenuOption;



void game_menu_update(Position_Direction dir, MenuOption *menuOption);

void game_start(MenuOption menuOption,
                Game_State *game_state,
                uint8_t *lives);

void game_lose_life(Game_State *game_state,
                    uint8_t *lives);

void game_back_to_menu(Game_State *game_state,
                       MenuOption menuOption);


#endif