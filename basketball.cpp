#ifndef BB_GAME

#define BB_GAME


#include<iostream>
#include<string.h>
#include "raylib.h"
#include "raymath.h"
#include "macros.h"

using namespace std;

//g++ prac.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main && ./main

float clamp(float val, float up_threshold, float down_threshold){
    if(val > up_threshold) return up_threshold;
    else if(val < down_threshold) return down_threshold;
    else return val;
}

class Sound_Effects{
    public:
        Sound ball_throw, ball_bounce, arm_rotation;

        void load(){
            ball_throw = LoadSound("Assets/Audios/Ball_throw.wav");
            ball_bounce = LoadSound("Assets/Audios/Ball_sound.wav");
            arm_rotation = LoadSound("Assets/Audios/Arm_rotation.wav");
        }

        void play_sound(char c){
            if(c == 't'){
                StopSound(ball_throw);
                PlaySound(ball_throw);
            }
            else if(c == 'b'){
                StopSound(ball_bounce);
                PlaySound(ball_bounce);
            }
            else if(c == 'r'){
                if(!IsSoundPlaying(arm_rotation)) PlaySound(arm_rotation);
            }
        }

        void stop_sound(char c){
            if(c == 'r') StopSound(arm_rotation);
        }

        void unload(){
            UnloadSound(ball_bounce);
            UnloadSound(ball_throw);
            UnloadSound(arm_rotation);
        }

};

Sound_Effects sfx;


class ArmBasest{
    public:
        Texture2D basest;
        float width, height;
        Vector2 pos;

        void load(char *path){
            basest = LoadTexture(path);
            width = basest.width*=global_scale , height = basest.height*=global_scale;
            pos = {0, SCREEN_HEIGHT-height};
        }


        void render(){
            DrawTexture(basest, pos.x, pos.y, WHITE);
        }

        void new_game(){
            pos = {0, SCREEN_HEIGHT-height};
        }

        void unload(){
            UnloadTexture(basest);
        }
};

class ArmBase{
    public:
        Vector2 pos = {SCREEN_WIDTH/2, SCREEN_HEIGHT*.75};
        
        float rotation = -50;
        Vector2 origin = {global_scale * 35, global_scale * 182};
        Texture2D base;
        Vector2 init_pos;

        float width;
        float height;

        void load(char *path){
            base = LoadTexture(path);
            width = base.width*=global_scale , height = base.height*=global_scale;
            pos = {SCREEN_WIDTH/4, SCREEN_HEIGHT-height};
        }

        void render(Vector2 basest_pos){
            pos = {basest_pos.x+global_scale *174, basest_pos.y+global_scale *54};
            Rectangle src = {0, 0, width, height};
            Rectangle dest = {pos.x, pos.y, width, height};

            DrawTexturePro(base, src, dest, origin, rotation, WHITE);
        }

        void rotate(float angle){
            rotation += angle;
            rotation = clamp(rotation, rotation_limit, -rotation_limit);
            if(rotation >= rotation_limit || rotation <= -rotation_limit) sfx.stop_sound('r');
        }

        void new_game(){
            rotation = -50;
        }

        void unload(){
            UnloadTexture(base);
        }
};

class ArmPart1{
    public:
        ArmBase base;
        Vector2 pos;
        
        Vector2 init_pos = {base.pos.x-global_scale * 4, base.pos.y};
        Texture2D arm;
        Rectangle arm_rec;

        float l = global_scale * 156.052127;
        float width, height;

        void load(char *path){
            arm = LoadTexture(path);
            arm.width *= global_scale;
            arm.height *= global_scale;
            width = arm.width;
            height = arm.height;
        }
        
        float rotation = 0;

        float self_rotation = 80;
        Vector2 origin = {global_scale * 34, global_scale * 172};

        void render(ArmBase base_part){
            base = base_part;
            init_pos = {base.pos.x-global_scale * 5, base.pos.y};
            rotation = base.rotation+self_rotation;
            

            pos.x = init_pos.x + l*sin(pi/180*(base.rotation));
            pos.y = init_pos.y - l*(cos(pi/180*(base.rotation)));

            Rectangle src = {0, 0, width, height};
            Rectangle dest = {pos.x, pos.y, width, height};

            DrawTexturePro(arm, src, dest, origin, rotation, WHITE);
            arm_rec = dest;
        }


        void rotate(float angle){
            self_rotation += angle;
            self_rotation = clamp(self_rotation, rotation_limit, -rotation_limit);
            if(self_rotation >= rotation_limit || self_rotation <= -rotation_limit) sfx.stop_sound('r');
        }

        void new_game(){
            rotation = 0;
            self_rotation = 80;
        }

        void unload(){
            UnloadTexture(arm);
        }
};

class ArmPart2{
    public:
        ArmPart1 arm1;
        Vector2 pos;
        Vector2 init_pos;
        Texture2D arm;

        float width;
        float height; 

        void load(char *path){
            arm = LoadTexture(path);
            arm.width *= global_scale;
            arm.height *= global_scale;
            width = arm.width;
            height = arm.height;
        }

        float self_rotation = 80;
        float rotation = 0;
        float l = global_scale *156.0527;
        Vector2 origin = {global_scale *57,global_scale * 187};

        void render(ArmPart1 arm1_part){
            arm1 = arm1_part;
            init_pos = (Vector2){arm1.pos.x, arm1.pos.y};
            rotation = arm1.rotation+self_rotation;

            pos.x = init_pos.x + l*sin(pi/180*(arm1.rotation));
            pos.y = init_pos.y - l*(cos(pi/180*(arm1.rotation)));

            Rectangle src = {0, 0, width, height};
            Rectangle dest = {pos.x, pos.y, width, height};

            DrawTexturePro(arm, src, dest, origin, rotation, WHITE);

        }

        void rotate(float angle){
            self_rotation += angle;
            self_rotation = clamp(self_rotation, rotation_limit, -rotation_limit);
            if(self_rotation >= rotation_limit || self_rotation <= -rotation_limit) sfx.stop_sound('r');
        }

        void new_game(){
            self_rotation = 80;
            rotation = 0;
        }

        void unload(){
            UnloadTexture(arm);
        }
};

class Basketball{
    public: 
        Texture2D ball;

        float width, height;
        float scale = 0.16;
        float l = (187-2)*global_scale;
        float rotation;
        bool released = false;
        int walls_collided[2] = {0, 0};

        float u, angle;

        Vector2 pos;
        Vector2 velocity = {0, 0};

        void load(char *path){
            ball = LoadTexture(path);
            width = ball.width *= global_scale*scale;
            height = ball.height *= global_scale*scale;
        }

        void shoot(float ball_u, float ball_angle){
            sfx.play_sound('t');
            u = ball_u;
            angle = ball_angle;
            velocity.x = u *cos(angle*DEG2RAD);
            velocity.y = u * sin(angle *DEG2RAD);
        }

        int collided_with_wall(){
            int collided = 0;
            for(int i = 0; i < 2; i++){
                if(CheckCollisionCircleRec(pos, width/2, walls[i])){
                    collided++;
                    walls_collided[i]++;
                }
            }
            return collided;
        }
        void render(ArmPart2 gripper){
            if(!released){
                pos.x = gripper.pos.x + l*sin(pi/180*(gripper.rotation)) ;
                pos.y = gripper.pos.y - l*(cos(pi/180*(gripper.rotation)));
                rotation = gripper.rotation;
            }
            else{
                pos.x += velocity.x * delta_time;
                pos.y += velocity.y * delta_time;

                velocity.y -= gravity*delta_time; 
                if(collided_with_wall() || pos.x + width/2  > SCREEN_WIDTH){
                    sfx.play_sound('b');
                    velocity.x = -velocity.x * 0.95;
                }
            }

            Rectangle src = {0, 0, width, height};
            Rectangle dest = {pos.x, pos.y, width, height};
            Vector2 origin = {width/2, height/2};

            DrawTexturePro(ball, src, dest, origin, rotation, WHITE);
        }

        void new_game(){
            rotation = 0;
            walls_collided[0] = 0;
            walls_collided[1] = 0;
            released = 0;
        }

        void unload(){
            UnloadTexture(ball);
        }
};

class Hoop{
    public:
        Texture2D hoop;
        float width, height;
        float scale = 0.5;
        Vector2 pos;
        Rectangle hoop_rec;

        void load(char *path){
            hoop = LoadTexture(path);
            width = hoop.width *= global_scale*scale;
            height = hoop.height *= global_scale*scale;
            pos = {SCREEN_WIDTH-width, SCREEN_HEIGHT/2-height};
        }

        void render(){
            walls[0] = (Rectangle){pos.x, pos.y, 2, height/2};
            walls[1] = (Rectangle){pos.x+width-5, pos.y, 2, height/2};
            DrawTexture(hoop, pos.x, pos.y, WHITE);
        }

        void unload(){
            UnloadTexture(hoop);
        }

};

class Button{
    public:
        float width, height;
        string text;
        float text_spacing = 2;
        float roundness;
        int font_size = 30*global_scale;
        bool hover_mode = true;
        Vector2 pos;
        Color color, normal_color, hover_color;
        Color border_color, normal_border_color, hover_border_color;
        Rectangle button_rec;

        Button(string &b_text, float b_width, float b_height, float x, float y, float round, Color b_color){
            text = b_text;
            width = b_width;
            height = b_height;
            pos.x = x - width/2;
            pos.y = y - height/2;
            normal_color = b_color;
            roundness = round;
            int d = 30;
            hover_color = { (unsigned char)(b_color.r + d), (unsigned char)(b_color.g + d), (unsigned char)(b_color.b + d), b_color.a };
            normal_border_color = DARKGRAY;
            hover_border_color = { (unsigned char)(normal_border_color.r + d), (unsigned char)(normal_border_color.g + d), (unsigned char)(normal_border_color.b + d), normal_border_color.a };
        }

        void render(){
            button_rec = {pos.x, pos.y, width, height};

            if(!CheckCollisionPointRec(cursor, button_rec) || !hover_mode){
                color = normal_color;
                border_color = normal_border_color;
            }
            else{
                color = hover_color;
                border_color = hover_border_color;
            }

            DrawRectangleRounded(button_rec, roundness, 5, color);
            DrawRectangleRoundedLinesEx(button_rec, roundness, 5, 5*global_scale, border_color);

            Vector2 text_size = MeasureTextEx(GetFontDefault(), text.c_str(), font_size, text_spacing);
            DrawTextEx(font, text.c_str(), (Vector2){pos.x+ width/2 - text_size.x/2, pos.y+ height/2 -text_size.y/2}, font_size, text_spacing, WHITE);

        }

        int clicked(){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(cursor, button_rec)){
                return 1;
            }
            return 0;
        }

};

string next_level_txt_b = "PROCEED";
Button next_level_btn_b(next_level_txt_b, 150, 50, SCREEN_WIDTH/2, SCREEN_HEIGHT/3 + 120, .2, RED);

void print_text_b(Font font, const char *text, Vector2 pos, float font_size, float spacing, float thickness, Color text_color, Color border_color, char align){
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


void next_game_screen_b(Texture2D bg){
    Font font = GetFontDefault();
    DrawTexturePro(bg,
            (Rectangle){0, 0,bg.width,bg.height},
            (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
            (Vector2){0,0}, 0, GRAY);
    
    Color text_box_col = GetColor(0xD3D3D3CC);
    Rectangle text_box = {SCREEN_WIDTH/2-250, SCREEN_HEIGHT/2-150, 500, 300};
    DrawRectangleRounded(text_box, 0.1, 5, text_box_col);

    print_text_b(font, "YOU MAY PROCEED", (Vector2){0, SCREEN_HEIGHT/3}, 30, 3, 0, BLACK, BLACK, 'c');
    print_text_b(font, "TO THE NEXT LEVEL", (Vector2){0, SCREEN_HEIGHT/3 + 40}, 30, 3, 0, BLACK, BLACK, 'c');
    
    next_level_btn_b.font_size = 25;
    next_level_btn_b.hover_mode = true;

    next_level_btn_b.render();


    if(next_level_btn_b.clicked()){
        current_screen = static_cast<GAME_SCREEN>(current_screen + 1);
        levels_cleared++;
    }

}


void DrawGrid2D(int rows, int cols, float cellSize, Color color) {
    for (int i = 0; i <= cols; i++) {
        DrawLine(i * cellSize, 0, i * cellSize, rows * cellSize, color);
    }
    for (int j = 0; j <= rows; j++) {
        DrawLine(0, j * cellSize, cols * cellSize, j * cellSize, color);
    }
}

void flash_bang(Texture2D pic){
    for(int i = 0; i < 60; i++){
        float y = -((i-30)*(i-30))/15 + 60;
        Color fade = {255, 255, 255, (100.0/60) *y};
        //DrawTextureEx(pic, (Vector2){0, 0}, 0, SCREEN_WIDTH/pic.width, fade);
        DrawTexturePro(pic, {0, 0, pic.width, pic.height}, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, {0, 0}, 0, WHITE);
        EndDrawing();
        WaitTime(delta_time);
    }
}

    Texture2D background;
    ArmBase base;
    ArmPart1 arm;
    ArmPart2 gripper;
    ArmBasest basest;
    Basketball basketball;
    Hoop hoop;

    Texture2D lebron;

    

    string shoot_btn_txt = "SHOOT!";
    Button shoot_button(shoot_btn_txt, 150*global_scale, 50*global_scale, SCREEN_WIDTH/2, SCREEN_HEIGHT/10, 1, RED);

    string score_txt = to_string(scored) + "/" + to_string(total_attempts);
    Button score_board(score_txt, 100*global_scale, 70*global_scale, SCREEN_WIDTH*.9, SCREEN_HEIGHT/10, 0.1, ORANGE);


 

void BBLoadAll(){
    background = LoadTexture("Assets/Basketball/Court.png");
    background.width = SCREEN_WIDTH;
    background.height = SCREEN_HEIGHT;
    lebron = LoadTexture("Assets/Basketball/Lebron.png");
    base.load("Assets/Robotic_Arm/Base.png");
    arm.load("Assets/Robotic_Arm/Arm1.png");
    gripper.load("Assets/Robotic_Arm/Arm2.png");
    basest.load("Assets/Robotic_Arm/Basest.png");
    basketball.load("Assets/Basketball/Basketball.png");
    hoop.load("Assets/Basketball/Hoop.png");
    sfx.load();
    score_board.hover_mode = false;
    score_board.font_size = 30*global_scale;
    walls[0].x = SCREEN_WIDTH;
    walls[1].x = SCREEN_WIDTH;
}

void BBUnloadAll(){
    arm.unload();
    gripper.unload();
    base.unload();
    basest.unload();
    basketball.unload();
    hoop.unload();
    sfx.unload();

    UnloadTexture(lebron);
}

void basketball_game(){

    
        DrawTexture(background, 0, 0, GRAY);
        
        delta_time = GetFrameTime();
        cursor = GetMousePosition();

        if(IsKeyPressed(KEY_UP)){
            selected_arm = clamp(selected_arm+1, 2, -1);
        }
        else if(IsKeyPressed(KEY_DOWN)){
            selected_arm = clamp(selected_arm-1, 2, -1);
        } 

        if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) sfx.play_sound('r');
        if(IsKeyReleased(KEY_LEFT) || IsKeyReleased(KEY_RIGHT)) sfx.stop_sound('r');

        
        if(selected_arm == 0){
            if(IsKeyDown(KEY_LEFT)){
                base.rotate(-rotation_angle);
            }
            else if(IsKeyDown(KEY_RIGHT)){
                base.rotate(rotation_angle);
            }
        }
        else if(selected_arm == 1){
            if(IsKeyDown(KEY_LEFT)){
                arm.rotate(-rotation_angle);
            }
            else if(IsKeyDown(KEY_RIGHT)){
                arm.rotate(rotation_angle);
            }
        }
        else if(selected_arm == 2){
            if(IsKeyDown(KEY_LEFT)){
                gripper.rotate(-rotation_angle);
            }
            else if(IsKeyDown(KEY_RIGHT)){
                gripper.rotate(rotation_angle);
            }
        }
        else if(selected_arm == -1){
            if(IsKeyDown(KEY_LEFT)){
                basest.pos.x -= movement_rate;
            }
            else if(IsKeyDown(KEY_RIGHT)){
                basest.pos.x += movement_rate;
            }
        }
        
        if(shoot_button.clicked() && !is_clicked){
            is_clicked = true;
        }
        if(is_clicked){
            frame_count++;
            arm.rotate(-0.025*frame_count);
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                is_clicked = false;
                basketball.released = true;
                basketball.shoot(950 + .5*frame_count, arm.rotation);
            }
        }


        bool scored_condition_1 = basketball.walls_collided[0] && basketball.walls_collided[1];
        bool scored_condition_2 = (basketball.pos.x >= walls[0].x && basketball.pos.x <= walls[1].x) && (basketball.pos.y >= walls[0].y && basketball.pos.y <= walls[0].y + walls[0].height);

        

        if(scored_condition_1 || scored_condition_2){
            game_won = 1 ;
            //cout << basketball.pos.x << " " << basketball.pos.y << endl;
            //cout << walls[0].x << " " << walls[1].x << " " << walls[0].y << " " << walls[0].y + hoop.height << endl;
        }

        if(basketball.released && (basketball.pos.x - basketball.width/2 > SCREEN_WIDTH || basketball.pos.y - basketball.height/2 > SCREEN_HEIGHT)){
            if(game_won){
                scored++;
                //flash_bang(lebron);
                
            } 

            attempt_no++;
            basest.new_game();
            base.new_game();
            arm.new_game();
            gripper.new_game();
            basketball.new_game(); 
            frame_count = 0;
            game_won = 0;
            selected_arm = -1;
        }

        string attempt_txt = "ATTEMPTS : " + to_string(attempt_no);

        DrawText(attempt_txt.c_str(), score_board.pos.x - 30*global_scale, score_board.pos.y - 30*global_scale, 20*global_scale, WHITE);

        score_txt =  to_string(scored) + "/" + to_string(total_attempts);
        score_board.text = score_txt;

        


        basest.render();
        arm.render(base);
        gripper.render(arm);
        base.render(basest.pos);
        basketball.render(gripper);
        hoop.render();
        shoot_button.render();
        score_board.render();

        if(attempt_no == 5 && scored < min_scores){
            current_screen = END_SCREEN;
        }
        else if(attempt_no == 5 && scored >= min_scores){
            levels_cleared++;
            current_screen = END_SCREEN;
        }
}


#endif