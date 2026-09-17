#ifndef E_SCREEN

#define E_SCREEN


#include<iostream>
#include<string.h>
#include "raylib.h"
#include "raymath.h"
#include "macros.h"
using namespace std;

Texture2D end_bg, you_won, game_over_screen;
float you_won_scale = 0.8;
float game_over_scale = 0.75;

void EndLoadAll(){
    end_bg = LoadTexture("Assets/End_Screen/End_bg.png");
    you_won = LoadTexture("Assets/End_Screen/You_won.png");
    game_over_screen= LoadTexture("Assets/End_Screen/Game_Over.png");

    you_won.width *= you_won_scale;
    you_won.height *= you_won_scale;

    game_over_screen.width *= game_over_scale;
    game_over_screen.height *= game_over_scale;

}


void ending_screen(){

    DrawTexturePro(end_bg, {0, 0, end_bg.width, end_bg.height}, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, {0, 0}, 0, LIGHTGRAY);

    if(levels_cleared == total_games){
        Vector2 you_won_pos = {(SCREEN_WIDTH-you_won.width)/2 , SCREEN_HEIGHT/4.5};
        DrawTexturePro(you_won, {0, 0, you_won.width, you_won.height}, {you_won_pos.x, you_won_pos.y, you_won.width, you_won.height}, {0, 0}, 0, WHITE);
    }
    else{
        Vector2 game_over_pos = {(SCREEN_WIDTH-game_over_screen.width)/2 , SCREEN_HEIGHT/4.5};
        DrawTexturePro(game_over_screen, {0, 0, game_over_screen.width, game_over_screen.height}, {game_over_pos.x, game_over_pos.y, game_over_screen.width, game_over_screen.height}, {0, 0}, 0, WHITE);

    }
} 

void EndUnloadAll(){
    UnloadTexture(end_bg);
    UnloadTexture(you_won);
    UnloadTexture(game_over_screen);
}

#endif