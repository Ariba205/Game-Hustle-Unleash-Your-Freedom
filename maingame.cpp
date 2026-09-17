#include<iostream>
#include<string.h>
#include "raylib.h"
#include "raymath.h"
#include "macros.h"
#include "drone.cpp"
#include "car.cpp"
#include "basketball.cpp"
#include "startscreen.cpp"
#include "endscreen.cpp"
using namespace std;

void LoadAllGameStuff(){
    BBLoadAll();
    CarLoadAll();
    load_texture();
    LoadAllSounds();
    StartLoadAll();
    EndLoadAll();
}

void UnloadAllGameStuff(){
    BBUnloadAll();
    UnloadTexture(Background);
    UnloadTexture(Drone);
    UnloadTexture(Box);
    UnloadMusicStream(background_sound);
    UnloadSound(pickdrop_sound);
    CarUnloadAll();
    StartUnloadAll();
    EndUnloadAll();
}

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "UNBOUND");
    SetTargetFPS(FPS);
    InitAudioDevice();
    SetRandomSeed((unsigned int)time(NULL));

    LoadAllGameStuff();

    PlayMusicStream(music);    
    PlayMusicStream(background_sound);
    

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        cursor = GetMousePosition();
        UpdateMusicStream(background_sound);

        
        if(current_screen == DRONE_GAME) {
            drone_game();
        } 
        else if(current_screen == BASKETBALL_GAME){
            basketball_game();
        }
        else if(current_screen == CAR_GAME){
            car_game();
        }
        else if(current_screen == START_SCREEN){
            starting_screen();
        }
        else if(current_screen == END_SCREEN){
            ending_screen();
        }

        EndDrawing();
    }

    
    
    UnloadAllGameStuff();
    
    CloseAudioDevice();
    CloseWindow();
}

