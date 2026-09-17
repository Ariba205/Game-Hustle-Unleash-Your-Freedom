#ifndef S_SCREEN

#define S_SCREEN


#include<iostream>
#include<string.h>
#include "raylib.h"
#include "raymath.h"
#include "macros.h"
using namespace std;

Texture2D start_bg, start_btn, logo;
float btn_scale = 0.3, logo_scale = 0.75;

void StartLoadAll(){
    start_bg = LoadTexture("Assets/Start_Screen/Start_bg.png");
    start_btn = LoadTexture("Assets/Start_Screen/Start_btn.png");
    logo = LoadTexture("Assets/Start_Screen/logo.png");


    logo.width *= logo_scale;
    logo.height *= logo_scale;

    start_btn.width*=btn_scale;
    start_btn.height*=btn_scale;
}


void starting_screen(){

    DrawTexturePro(start_bg, {0, 0, start_bg.width, start_bg.height}, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, {0, 0}, 0, GRAY);

    Vector2 btn_pos = {(SCREEN_WIDTH-start_btn.width)/2 , SCREEN_HEIGHT/1.75};
    DrawTexturePro(start_btn, {0, 0, start_btn.width, start_btn.height}, {btn_pos.x, btn_pos.y, start_btn.width, start_btn.height}, {0, 0}, 0, WHITE);

    Rectangle btn_rec = {btn_pos.x, btn_pos.y, start_btn.width, start_btn.height};

    Vector2  logo_pos = {(SCREEN_WIDTH-logo.width)/2, SCREEN_HEIGHT/3.5-logo.height/2};
    DrawTexturePro(logo, {0, 0, logo.width, logo.height}, {logo_pos.x, logo_pos.y, logo.width, logo.height}, {0, 0}, 0, WHITE);

    if(CheckCollisionPointRec(cursor, btn_rec)) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if(CheckCollisionPointRec(cursor, btn_rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        current_screen = DRONE_GAME;
    }
}

void StartUnloadAll(){
    UnloadTexture(start_bg);
}

#endif