#ifndef MACROS
#define MACROS


#define SCREEN_WIDTH (float)(960)
#define SCREEN_HEIGHT (float)(600)
#define FPS 60

#define gravity -1200
#define pi 3.14159

float delta_time= 1/60.0;

float rotation_angle = 2.5;
float movement_rate = 5;
int selected_arm = -1;
float rotation_limit = 80;
float global_scale = 0.25 + (SCREEN_HEIGHT>SCREEN_WIDTH ? (SCREEN_WIDTH/SCREEN_HEIGHT) : (SCREEN_HEIGHT/SCREEN_WIDTH));
float arm_rotation_angle = 0;

bool is_clicked = false;

int frame_count = 0;
int game_won = 0;

int total_attempts = 5;
int attempt_no = 0;
int min_scores = 3;
int scored = 0;
int levels_cleared = 0;
int total_games = 3;

Vector2 cursor;
Font font = GetFontDefault();

Rectangle walls[2];

enum GAME_SCREEN{
    START_SCREEN,
    DRONE_GAME,
    CAR_GAME,
    BASKETBALL_GAME,
    //FOOTBALL_GAME,
    END_SCREEN
};

enum GAME_SCREEN current_screen = START_SCREEN;



#endif