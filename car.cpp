#ifndef C_GAME

#define C_GAME


#include "raylib.h"
#include "raymath.h"
#include "drone.cpp"
#include <iostream>
#include <stdbool.h>
#include <stdlib.h>
#include<string.h>
#include<time.h>

using namespace std;

#define WIDTH 960 
#define HEIGHT 600 

//#define NUM_LEVEL 5
#define MAX_SPEED_X 300
#define MAX_SPEED_y 300 
#define FLAG_NUM 12
#define ROTATION_SPEED 720.0f   
#define PATH_THICKNESS 50.0
#define NODE_RADIUS (PATH_THICKNESS/2.0)

#define NODE_COUNT 14
#define TRACK_NODE_COUNT (NODE_COUNT) 
#define TRACK_SEGMENT_COUNT (NODE_COUNT-1)
#define BOMB_NUM 5

typedef enum {
   // SCREEN_START,
   // SCREEN_MENU,
    SCREEN_GAMEPLAY,
    SCREEN_GAME_OVER
} Screen;

typedef struct {
    bool isActive;
    Vector2 pos;
    Vector2 posA, posB; 
    float t;            
    float speed; 
    Texture2D bomb_pic;
    Vector2 bombCenter;
} bomb;

bomb Bomb[BOMB_NUM];

typedef struct{
 bool istaken;
 Vector2 pos;
 int point;
 Texture2D flag_pic;
 Vector2 flagCenter ;
}flag;

bool is_pressed(Vector2 mousepos, bool mousepressed, Rectangle rect) {
    if (CheckCollisionPointRec(mousepos,rect) && mousepressed) {
        return true;
    }
    return false;
}

Music music;
Sound sound1;
Sound sound2;
Sound sound3;
Sound sound4;

Screen currentScreen = SCREEN_GAMEPLAY;
int score=0;
int life=3;
float ground = 98.0 * HEIGHT / 100;
float w = 40;
float h = 40;
float bomb_w=30;
float bomb_h=30;
float animTimer = 0.0; 
float frameDuration = 0.4; 
float flag_w=40;
float flag_h=40;

bool paused=false;
bool game_over=false;
bool isCrashed = false;
bool iswon=false;

float respawnTimer =0.0f;
const float RESPAWN_DELAY =1.0f;
   
  


Vector2 int_position={100,480};
Vector2 position = {100,480};
Vector2 dest={660,480};
Vector2 speed = Vector2Zero();

float carAngle=0.0f; 
float targetCarAngle=0.0f;
float bombSwing = 35.0f; 

Vector2 position_life={(WIDTH)-w/2,10};
//Vector2 positionstart= {WIDTH*2/5,HEIGHT*1/2};
//Rectangle startButtonRect ={positionstart.x, positionstart.y, WIDTH/5, HEIGHT/4.5 };
 
flag Flag[FLAG_NUM];

Vector2 Nodes[NODE_COUNT] = {
    { 120,  460 },
    { 120, 120 },
    { 260, 120 },
    { 260, 300 },
    { 400, 300 },
    { 400, 360 },
    { 260, 360 },
    { 260, 460 },
    { 540, 460 },
    { 540, 220 },
    { 400, 220 },
    { 400, 120 },
    { 680, 120 },
    { 680, 460 },
};

Vector2 trackNodes[TRACK_NODE_COUNT];
Rectangle trackSegments[TRACK_SEGMENT_COUNT];
int segIndex = 0;

Texture2D start, startbutton, racetrack, car, Life, destination, win, your_score_text, Game_over_back, game_over_text, play_again, bomb_picture;


Rectangle SegmentRect(Vector2 a,Vector2 b,float thickness) {
    float x =fminf(a.x,b.x)-thickness/2.0;
    float y =fminf(a.y, b.y)-thickness/2.0;
    float w =fabsf(b.x-a.x)+thickness;
    float h =fabsf(b.y-a.y)+thickness;
    return (Rectangle){x,y,w,h};
}

bool CheckCarOnTrack(Rectangle carRect,Rectangle *segments,int segCount,
                      Vector2 *nodes,int nodeCount,float nodeRadius) {
    for (int i = 0; i < segCount; i++) {
        if (CheckCollisionRecs(carRect,segments[i])) return true;
    }
    for (int i = 0; i < nodeCount; i++) {
        if (CheckCollisionCircleRec(nodes[i], nodeRadius, carRect)) return true;
    }
    return false;
}

void CarLoadAll(){
  // start=LoadTexture("asset_for_gamejam/background/Race_track_bac.png");
   //startbutton=LoadTexture("asset_for_gamejam/Menu/Buttons/Start_button_new.png");
   racetrack=LoadTexture("asset_for_gamejam/background/Race_track.png");
   car=LoadTexture("asset_for_gamejam/Car/Car_new.png");
   Life= LoadTexture("asset_for_gamejam/Flag/Life5.png");
   destination=LoadTexture("asset_for_gamejam/Flag/golden_trophy.png");
   win=LoadTexture("asset_for_gamejam/Button/Win.png");
   your_score_text=LoadTexture("asset_for_gamejam/Button/your_score_new.png");
   Game_over_back=LoadTexture("asset_for_gamejam/background/game_over.png");
   game_over_text=LoadTexture("asset_for_gamejam/Button/Game_over_new.png");
   play_again=LoadTexture("asset_for_gamejam/Button/play_again.png");
   bomb_picture = LoadTexture("asset_for_gamejam/Flag/bomb.png");

   music=LoadMusicStream("asset_for_gamejam/Sound/The Rush.mp3");
  sound1=LoadSound("asset_for_gamejam/Sound/powerup.wav");
   sound2=LoadSound("asset_for_gamejam/Sound/error.ogg");
  sound3=LoadSound("asset_for_gamejam/Sound/win_sound.wav");
  sound4=LoadSound("asset_for_gamejam/Sound/gameoverdark.wav");

  for (int j = 0; j <FLAG_NUM; j++) {
    char path[50];
    sprintf(path, "asset_for_gamejam/Flag/flag%d.png",(j%5)+1);
    Flag[j].flag_pic = LoadTexture(path);
  } 
   for(int i=0;i<FLAG_NUM;i++)
  {
    Flag[i].istaken=false;
    Flag[i].pos=(Vector2){Nodes[i+1].x-flag_w/2.0,Nodes[i+1].y-flag_h/2.0};

  }
  
for (int i = 0; i < BOMB_NUM; i++) {
    Bomb[i].isActive = true;
    Bomb[i].bomb_pic = bomb_picture;
   
}
Bomb[0].posA=(Vector2){120-bombSwing, 280};
Bomb[0].posB=(Vector2){120+bombSwing, 280};
Bomb[0].speed=1.8f;

Bomb[1].posA=(Vector2){400-bombSwing,160};
Bomb[1].posB=(Vector2){400+bombSwing,160};
Bomb[1].speed=2.2f;

Bomb[2].posA = (Vector2){260 - bombSwing, 400};
Bomb[2].posB = (Vector2){260 + bombSwing, 400};
Bomb[2].speed = 2.0f;

Bomb[3].posA = (Vector2){540 - bombSwing, 360};
Bomb[3].posB = (Vector2){540 + bombSwing, 360};
Bomb[3].speed = 1.6f;

Bomb[4].posA = (Vector2){680 - bombSwing, 300};
Bomb[4].posB = (Vector2){680 + bombSwing, 300};
Bomb[4].speed = 2.4f;

for (int i = 0; i < BOMB_NUM; i++) {
    Bomb[i].pos = Bomb[i].posA;
    Bomb[i].t = (float)i*1.3f;  
}
for (int i = 0; i < NODE_COUNT; i++) trackNodes[i]=Nodes[i];
 
  
  for (int i = 0; i < NODE_COUNT-1; i++)
    trackSegments[segIndex++] = SegmentRect(Nodes[i],Nodes[i + 1], PATH_THICKNESS);


 
}


void CarUnloadAll(){
//  UnloadTexture(start);
 // UnloadTexture(startbutton);
  UnloadTexture(racetrack);
  UnloadTexture(car);
  UnloadTexture(Life);
  UnloadTexture(Game_over_back);
  UnloadTexture(game_over_text);
  UnloadMusicStream(music);
  UnloadSound(sound1);
  UnloadSound(sound2);
  UnloadSound(sound3);
  UnloadSound(sound4);
}


void car_game(){
    UpdateMusicStream(music);
  
   if(paused)
   {
    PauseMusicStream(music);
   }
   else
   {
    ResumeMusicStream(music);
   }
      Vector2 mouseposition = GetMousePosition();

        bool mousepressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        bool onTrack = true;
     switch(currentScreen)
     {
      /*
       case SCREEN_START:

        if (is_pressed(mouseposition, mousepressed, startButtonRect))
        {
            currentScreen = SCREEN_GAMEPLAY;
        }

        break;
      */
        

        case SCREEN_GAMEPLAY:
        if (IsKeyPressed(KEY_P))
        {
            paused = !paused;
        }
        break;

        case SCREEN_GAME_OVER:
        {
           float ratio3=(float)play_again.width /play_again.height;
           Rectangle play_again_ButtonRect={ WIDTH/2-WIDTH*0.3f/2,HEIGHT-80, WIDTH*0.3f/ratio3 };
        
        
        if (is_pressed(mouseposition, mousepressed, play_again_ButtonRect))
        {
           
    game_over = false;
    paused = false;
    iswon = false;
    score = 0;
    life = 3;
    isCrashed = false;
    respawnTimer = 0.0f;
    carAngle = 0.0f;
    targetCarAngle = 0.0f;
    for (int i = 0; i < FLAG_NUM; i++) Flag[i].istaken = false;
    position = int_position;
    speed = Vector2Zero();
    for (int i = 0; i < BOMB_NUM; i++) Bomb[i].isActive = true;
    currentScreen = SCREEN_GAMEPLAY;
        }
       
        break;
    }
        }
    
     if (currentScreen == SCREEN_GAMEPLAY)
    {
      
    if(!paused && !game_over)

  {
   float dt = GetFrameTime();
  
   
      if (isCrashed)
     {
    respawnTimer-=dt;
    if (respawnTimer<=0.0f)
    {
        isCrashed=false;
        if (life <= 0)
        {
            game_over = true;
            PlaySound(sound4);
            currentScreen = SCREEN_GAME_OVER;
        }
        else
        {
            position = int_position;
            speed = Vector2Zero();
        }
    }
}

else
{
float angleDiff=targetCarAngle-carAngle;

while (angleDiff > 180.0f)  angleDiff-=360.0f;
while (angleDiff < -180.0f) angleDiff+=360.0f;

float maxStep = ROTATION_SPEED*dt;
if (fabsf(angleDiff)<=maxStep) {
    carAngle = targetCarAngle;        
} else {
    carAngle+= (angleDiff> 0?maxStep:-maxStep);
}

if (carAngle < 0) carAngle+=360.0f;
if (carAngle >= 360.0f) carAngle-=360.0f;


  animTimer += dt;
 
    position = Vector2Add(position, Vector2Scale(speed, dt));
    
    if (position.x<0) {
      position.x=0;
    } else if (position.x+w>WIDTH)
      position.x = WIDTH - w;

    if (position.y < h) {
      position.y = h;
    } else if (position.y  >HEIGHT )
      position.y = HEIGHT;


   if (IsKeyDown(KEY_RIGHT)) {
      speed.x = MAX_SPEED_X;
       speed.y = 0;
      targetCarAngle= 90.0f;
       
    } else if (IsKeyDown(KEY_LEFT)) {
      speed.x = -MAX_SPEED_X;
    speed.y = 0;
    targetCarAngle= 270.0f;
    }
    else if(IsKeyDown(KEY_UP))
    {
    speed.y = -MAX_SPEED_y;
    speed.x = 0;
   targetCarAngle = 0.0f;
       
    }
    else if(IsKeyDown(KEY_DOWN))
    {
      speed.y = MAX_SPEED_y;
    speed.x = 0;
    targetCarAngle=180.0f;
    } 
    else {
      speed.x = 0;
      speed.y=0;
    }

  
    Rectangle carRect={position.x,position.y-h,w,h};
    onTrack = CheckCarOnTrack(carRect, trackSegments, TRACK_SEGMENT_COUNT,
                               trackNodes, TRACK_NODE_COUNT, NODE_RADIUS);

    
    
     for (int i = 0; i <FLAG_NUM; i++) {
    if (Flag[i].istaken) continue;

     Flag[i].flagCenter=(Vector2){
        Flag[i].pos.x + flag_w/2, Flag[i].pos.y+flag_h/2
    };

     }

     float flag_rad= (flag_w/2.0)/1.5;
    
     Vector2 carCenter = {
    position.x + w/2.0f,
    position.y - h/2.0f
       };
        float carRadius=(w/1.5)/2.0;
      
       Vector2 desCenter = {
           dest.x+(45)/2.0,
           dest.y+(45)/2.0
        };
        float desRadius=(45/2.0)/2.0;
       
        if (CheckCollisionCircles(carCenter,carRadius,desCenter,desRadius))
        {
          PlaySound(sound3);
            score+=500;
            iswon=true;
           currentScreen=SCREEN_GAME_OVER;
            
        }

        float bomb_rad=(bomb_w/2.0f)/1.1f;
for (int i = 0; i < BOMB_NUM; i++) {
    if (!Bomb[i].isActive) continue;


   Bomb[i].t+= Bomb[i].speed*dt;
   float s = (sinf(Bomb[i].t) + 1.0f)/2.0f;
   Bomb[i].pos = Vector2Lerp(Bomb[i].posA, Bomb[i].posB, s);

    Bomb[i].bombCenter = (Vector2){ Bomb[i].pos.x + bomb_w/2, Bomb[i].pos.y + bomb_h/2 };
    if (CheckCollisionCircles(carCenter, carRadius, Bomb[i].bombCenter, bomb_rad)) {
        Bomb[i].isActive = false;
        isCrashed=true;
        respawnTimer=RESPAWN_DELAY;
        PlaySound(sound2);             
        score -= 50;                   
        life--;
        speed = Vector2Zero();
    }
}


      for (int i = 0; i <FLAG_NUM; i++) {
    if (!Flag[i].istaken)
    { if (CheckCollisionCircles(carCenter,carRadius,Flag[i].flagCenter,flag_rad))
        {
          PlaySound(sound1);
            Flag[i].istaken=true;
          
            score+=50;
            break;
            
        }
      }
     }
        
    if(!onTrack)
    {
        isCrashed = true;
        respawnTimer = RESPAWN_DELAY;
        PlaySound(sound2);
        score -= 20;
        life--;
        speed = Vector2Zero();

    }
   
  }
  
  
}
if (life <= 0 || iswon==true) {
   game_over=true;
   PlaySound(sound4);
   currentScreen = SCREEN_GAME_OVER;
}

}



   

    BeginDrawing();
    ClearBackground((Color){102, 191, 255, 255});

     switch (currentScreen)
  {
    /*
    case SCREEN_START:

       DrawTexturePro(start,
                       (Rectangle){0, 0, start.width, start.height},
                       (Rectangle){0, 0, WIDTH, HEIGHT},
                       Vector2Zero(), 0, WHITE);
                    

       
       DrawTexturePro(startbutton,
                       (Rectangle){0, 0, startbutton.width, startbutton.height},
                       startButtonRect,
                       Vector2Zero(), 0, WHITE);
       
        
       break;

    */
    
      case SCREEN_GAMEPLAY:
      {
         

          DrawTexturePro(racetrack,
                   (Rectangle){0, 0, racetrack.width,racetrack.height},
                   (Rectangle){0, 0,WIDTH, HEIGHT},
                   Vector2Zero(), 0, WHITE);
    
        for (int i = 0; i < TRACK_SEGMENT_COUNT; i++)
        {
        
         DrawRectangleRounded(trackSegments[i], 0.3, 8, (Color){ 255, 203, 0, 235 });
        }
       
        for (int i = 0; i < TRACK_NODE_COUNT; i++)
        {
          DrawCircleV(trackNodes[i], NODE_RADIUS, (Color){ 255, 203, 0, 235 });
        }
       
        for (int i = 0; i < TRACK_SEGMENT_COUNT; i++)
        {
        
           DrawRectangleRoundedLinesEx(trackSegments[i], 0.3, 8, 2, (Color){10,17,40,255});
        }

        for(int i=0;i<FLAG_NUM;i++)
        {
          if(Flag[i].istaken==false)
          {
              DrawTexturePro(Flag[i].flag_pic,
                   (Rectangle){0, 0,Flag[i].flag_pic.width,Flag[i].flag_pic.height},
                   (Rectangle){Flag[i].pos.x,Flag[i].pos.y,flag_w,flag_h},
                   Vector2Zero(), 0, WHITE);
          }
        }
      
        Color carTint =!isCrashed? WHITE : (Color){255,120,120,255};
         position_life.x = WIDTH - w/2;
    for(int i=0;i<life;i++)
    {
        DrawTexturePro(Life,
                   (Rectangle){0, 0, Life.width, Life.height},
                   (Rectangle){position_life.x, position_life.y,30,30},
                   Vector2Zero(),0, WHITE);

        position_life.x-=(35);
    }
     if(!iswon)
     {
       DrawTexturePro(destination,
                   (Rectangle){0, 0, destination.width, destination.height},
                   (Rectangle){dest.x, dest.y,45,45},
                   Vector2Zero(),0, WHITE);
     }
    
    for (int i = 0; i < BOMB_NUM; i++) {
    if (Bomb[i].isActive) {
        DrawTexturePro(Bomb[i].bomb_pic,
            (Rectangle){0,0,Bomb[i].bomb_pic.width,Bomb[i].bomb_pic.height},
            (Rectangle){Bomb[i].pos.x, Bomb[i].pos.y, bomb_w, bomb_h},
            Vector2Zero(), 0, WHITE);
    }
}

    Vector2 carOrigin = { w/2.0f,w/2.0f };
   Rectangle carDest = {
    position.x + w / 2.0f,
    (position.y - w) + w / 2.0f,
    w, w
    };

DrawTexturePro(car,
           (Rectangle){0, 0, car.width, car.height},
           carDest,
           carOrigin, carAngle, carTint);
 
         Rectangle groundRect = {WIDTH/80, HEIGHT/60, 200, 35};
     
    DrawRectangleRec(groundRect,YELLOW); 
    DrawRectangleLinesEx(groundRect, 3,BLACK);

     DrawText(TextFormat("SCORE-%04d", score), WIDTH/80 +2, HEIGHT/60 +2, 30, BLACK);

      if(paused)
    {
      DrawText("Paused", WIDTH/2-30, HEIGHT/2, 20, YELLOW);

    }
        break;
      }
       case SCREEN_GAME_OVER:
    {
      if(life <= 0) current_screen = END_SCREEN;
      else next_game_screen(Game_over_back);

     /* DrawTexturePro(Game_over_back,
                       (Rectangle){0, 0, Game_over_back.width, Game_over_back.height},
                       (Rectangle){0, 0, WIDTH, HEIGHT},
                       Vector2Zero(), 0, WHITE);
      
     
                       
     float ratio1=(float)game_over_text.width / game_over_text.height;
     Rectangle gameOverRect = { WIDTH/2 - WIDTH*0.5f/2, HEIGHT * 0.08f, WIDTH*0.5f, WIDTH*0.5f/ratio1 };

     DrawTexturePro(game_over_text,
                       (Rectangle){0, 0, game_over_text.width, game_over_text.height},
                       (Rectangle) gameOverRect,
                       Vector2Zero(), 0,WHITE);
        if(!iswon)
        {
         float ratio3 = (float)play_again.width /play_again.height;
    Rectangle play_again_ButtonRect ={WIDTH/2 - WIDTH*0.3f/2,HEIGHT - 80, WIDTH*0.3f/ratio3 };
           DrawTexturePro(play_again,
                       (Rectangle){0, 0, play_again.width, play_again.height},
                       play_again_ButtonRect, Vector2Zero(), 0, WHITE);
        }

        if(iswon)
   {
    float ratio4 = (float)win.width / win.height;
    Rectangle win_ButtonRect = { WIDTH/2 - WIDTH*0.3f/2, HEIGHT - 180, WIDTH*0.3f, WIDTH*0.3f/ratio4 };
    DrawTexturePro(win,
                   (Rectangle){0, 0, win.width, win.height},
                   win_ButtonRect, Vector2Zero(), 0, WHITE);
    }

    float ratio2 = (float)your_score_text.width / your_score_text.height;
        Rectangle yourScoreRect = { WIDTH/2 - WIDTH*0.3f/2, HEIGHT/2 -80, WIDTH*0.3f, WIDTH*0.3f/ratio2 };
        DrawTexturePro(your_score_text,
                       (Rectangle){0, 0, your_score_text.width, your_score_text.height},
                       yourScoreRect, Vector2Zero(), 0, WHITE);

       
        DrawText(TextFormat("%d", score),
                 yourScoreRect.x + yourScoreRect.width/2 - MeasureText(TextFormat("%d", score), 50)/2,
                 yourScoreRect.y + yourScoreRect.height + 10, 50, WHITE);

       */
 
        break;

} 
    
        default: printf("No screen opened\n");

        }

}



#endif