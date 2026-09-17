#ifndef DR_GAME

#define DR_GAME

#include"raylib.h"
#include "raymath.h"
#include "macros.h"
#include "basketball.cpp"
#include<string.h>
#include<iostream>

using namespace std;

#define WIDTH 960
#define HEIGHT 600

Texture2D Background;
Texture2D Drone; 
Texture2D Box;
Music background_sound;
Sound pickdrop_sound;

#define TOTAL_PARCELS 5

typedef struct Parcel {
    Vector2 position;
    float w, h;
    bool collected;
    bool carried;
} Parcel;


void LoadAllSounds()
{
    background_sound = LoadMusicStream("Assets/background_music_new.wav");
    SetMusicVolume(background_sound,0.7f);
    pickdrop_sound=LoadSound("Assets/drop.wav");
    SetSoundVolume(pickdrop_sound,1.0f);

}

Vector2 drone_pos = { WIDTH /2-100, HEIGHT/3-50 };
    float d_speed = 300.0f;
    int w_d = 230, h_d = 100;

    int w_b = 120, h_b = 90; 
    float ground_y = HEIGHT - h_b - 30; 
    float x_offsets[5] = { 30, 60, 100, 150, 40 };
    float y_offsets[5] = { 0, -10, -10, -5, -5 }; 

    Rectangle dropzone = { WIDTH - 200, 260, 150, 260};

    Parcel parcels[TOTAL_PARCELS];
    

    bool carrying_parcel = false;
    int  carry   = -1;
    int  deli_count = 0;

    float cooldown = 0;
    float grabarea = 80;


void load_texture()
{
   Background = LoadTexture("Assets/bg.png");
   Drone      = LoadTexture("Assets/drone5.png");
   Box        = LoadTexture("Assets/box1.png");

   for (int i = 0; i < TOTAL_PARCELS; i++)
    {
        parcels[i].position  = (Vector2){ x_offsets[i], ground_y + y_offsets[i] };
        parcels[i].w         = w_b;
        parcels[i].h         = h_b;
        parcels[i].collected = false;
        parcels[i].carried   = false;
    }
}

string next_level_txt = "PROCEED";
Button next_level_btn(next_level_txt, 150, 50, SCREEN_WIDTH/2, SCREEN_HEIGHT/3 + 120, .2, RED);

void print_text(Font font, const char *text, Vector2 pos, float font_size, float spacing, float thickness, Color text_color, Color border_color, char align){
    Vector2 text_size = MeasureTextEx(font, text, font_size, spacing);
    Vector2 origin = {0, 0};

    if(align == 'c'){
        pos.x = (SCREEN_WIDTH-text_size.x)/2.0;
    }
    else if(align == 'l'){
        pos.x = 0;
    }
    else if(align == 'r'){
        pos.x = SCREEN_WIDTH-text_size.x;
    }
    
    for(int i = 0; i < 4; i++){
        Vector2 temp_pos = pos;
        temp_pos.x += thickness*((i==0 || i == 1)*pow(-1, i));
        temp_pos.y += thickness*((i==2 || i == 3)*pow(-1, i));
        DrawTextPro(font, text, temp_pos, origin, 0, font_size, spacing, border_color);
    }
    DrawTextPro(font, text, pos, origin, 0, font_size, spacing, text_color);

}


void next_game_screen(Texture2D bg){
    Font font = GetFontDefault();
    DrawTexturePro(bg,
            (Rectangle){0, 0,bg.width,bg.height},
            (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
            (Vector2){0,0}, 0, GRAY);
    
    Color text_box_col = GetColor(0xD3D3D3CC);
    Rectangle text_box = {SCREEN_WIDTH/2-250, SCREEN_HEIGHT/2-150, 500, 300};
    DrawRectangleRounded(text_box, 0.1, 5, text_box_col);

    print_text(font, "YOU MAY PROCEED", (Vector2){0, SCREEN_HEIGHT/3}, 30, 3, 0, BLACK, BLACK, 'c');
    print_text(font, "TO THE NEXT LEVEL", (Vector2){0, SCREEN_HEIGHT/3 + 40}, 30, 3, 0, BLACK, BLACK, 'c');
    
    next_level_btn.font_size = 25;
    next_level_btn.hover_mode = true;

    next_level_btn.render();


    if(next_level_btn.clicked()){
        current_screen = static_cast<GAME_SCREEN>(current_screen + 1);
        levels_cleared++;
    }

}



void drone_game()
{
    
        float dt = GetFrameTime();
        if (cooldown > 0) cooldown -= dt;
        
        //UpdateMusicStream(background_sound);
    
        if (IsKeyDown(KEY_RIGHT)) drone_pos.x += d_speed * dt;
        if (IsKeyDown(KEY_LEFT))  drone_pos.x -= d_speed * dt;
        if (IsKeyDown(KEY_UP))    drone_pos.y -= d_speed * dt;
        if (IsKeyDown(KEY_DOWN))  drone_pos.y += d_speed * dt;


        if (drone_pos.x < 0) drone_pos.x = 0;
        if (drone_pos.x > WIDTH - w_d) drone_pos.x = WIDTH - w_d;
        if (drone_pos.y < 0) drone_pos.y = 0;
        if (drone_pos.y > HEIGHT - h_d) drone_pos.y = HEIGHT - h_d;

        
        Rectangle dronebody = { drone_pos.x, drone_pos.y, w_d, h_d };
        Vector2 claw = { drone_pos.x + w_d / 2, drone_pos.y + h_d-15};
        Vector2 dronecenter = { drone_pos.x + w_d / 2, drone_pos.y + h_d / 2 };

        
        if (IsKeyPressed(KEY_SPACE))
        {
            if (!carrying_parcel && cooldown <= 0)
            {
                for (int i = 0; i < TOTAL_PARCELS; i++)
                {
                    if (parcels[i].collected || parcels[i].carried) continue;

                    Rectangle parcelbody = {parcels[i].position.x,parcels[i].position.y,parcels[i].w,parcels[i].h};

                    if (CheckCollisionRecs(dronebody, parcelbody))
                    {
                        PlaySound(pickdrop_sound);
                        parcels[i].carried = true;
                        carrying_parcel = true;
                        carry = i;
                        break;
                    }
                }
            }
            else if (carrying_parcel)
            {
                
                parcels[carry].carried = false;
                parcels[carry].position = (Vector2){claw.x - parcels[carry].w / 2,claw.y + 2};
                carrying_parcel = false;
                carry = -1;
                cooldown = 0.4;   
            }
        }

        
        if (carrying_parcel)
        {
           
            parcels[carry].position = (Vector2){ claw.x - parcels[carry].w / 2,claw.y + 2 };

            if (CheckCollisionPointRec(dronecenter, dropzone))
            {
                PlaySound(pickdrop_sound);
                parcels[carry].collected = true;
                parcels[carry].carried   = false;
                carrying_parcel = false;
                deli_count++;
                carry = -1;
            }
        }
       

        DrawTexturePro(Background,
            (Rectangle){0, 0,Background.width,Background.height},
            (Rectangle){0, 0, WIDTH, HEIGHT},
            (Vector2){0,0}, 0, LIGHTGRAY);

  
        DrawTexturePro(Drone,
            (Rectangle){0, 0, Drone.width, Drone.height},
            (Rectangle){drone_pos.x, drone_pos.y, w_d, h_d},
            (Vector2){0,0}, 0,WHITE);


        DrawRectangleRec(dropzone, Fade(RED, 0.3f));
        DrawRectangleLinesEx(dropzone, 2,(Color){170, 4, 4,255});
        DrawText("DROP ZONE", dropzone.x + 5, dropzone.y - 25, 25,(Color){170, 4, 4,255});

        for (int i = 0; i < TOTAL_PARCELS; i++)
        {
            if (parcels[i].collected || parcels[i].carried) continue;
            DrawTexturePro(Box,
                (Rectangle){0, 0, Box.width, Box.height},
                (Rectangle){parcels[i].position.x, parcels[i].position.y,
                            parcels[i].w, parcels[i].h},
                (Vector2){0,0}, 0, WHITE);
        }

        
        if (carrying_parcel)
        {
            DrawTexturePro(Box,
                (Rectangle){0, 0, Box.width, Box.height},
                (Rectangle){parcels[carry].position.x,
                            parcels[carry].position.y,
                            parcels[carry].w,
                            parcels[carry].h},
                (Vector2){0,0}, 0, LIGHTGRAY);
        }

        
        DrawText(TextFormat("Delivered: %d / %d", deli_count, TOTAL_PARCELS),
                 10, HEIGHT - 30, 25, (Color){170, 4, 4,255});

        if (carrying_parcel)
            DrawText("Carrying a parcel!",20,20, 20, (Color){170, 4, 4,255});

        else{
            for (int i = 0; i < TOTAL_PARCELS; i++)
            {
                if (parcels[i].collected || parcels[i].carried) continue;

                Vector2 par_cel = {parcels[i].position.x + parcels[i].w / 2,parcels[i].position.y + parcels[i].h / 2 };

                if (Vector2Distance(claw, par_cel) < grabarea)
                {
                   const char *text = "Press SPACE to grab";
                   int tw = MeasureText(text, 20);

                DrawText(text,
                     (drone_pos.x + w_d / 2) - tw / 2,
                     drone_pos.y - 30,
                     20, YELLOW);
                    break;
                }
            }
        }

        if(deli_count == 5){
            next_game_screen(Background);
        }
}


#endif