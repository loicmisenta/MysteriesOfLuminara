#include <nds.h>
#include "wifi.h"

//---------------------------------------------------------------------
// The player is a sprite that can move in 4 directions
//---------------------------------------------------------------------
typedef struct{
    int right_collide_x;
    int right_collide_y;
    int left_collide_x;
    int left_collide_y;
    int top_collide_x;
    int top_collide_y;
    int bottom_collide_x;
    int bottom_collide_y;
}PlayerCollision;

typedef struct {
    int x, y; //position on the screen
    int player_pos_x; //position on the background
    int player_pos_y; //position on the background
    int direction;
    int speed;

    u16* sprite_gfx_mem[16]; // 16 16x32 tiles
    int gfx_frame;

    int anim_frame;
    PlayerCollision playerCollision;
} Player;


//define a player sprite for the remote player
typedef struct{
    int x, y;
    int player_pos_x;
    int player_pos_y;
    int direction;
    int speed;

    u16* sprite_gfx_mem[16]; // 16 16x32 tiles
    int gfx_frame;

    int anim_frame;
    int is_hidden;
} Player_remote;

//---------------------------------------------------------------------
// The state of the sprite (which way it is walking)
//---------------------------------------------------------------------
enum State{
    PLAYER_DIRECTION_DOWN,
    PLAYER_DIRECTION_RIGHT,
    PLAYER_DIRECTION_UP,
    PLAYER_DIRECTION_LEFT
};

enum PlayerNb{
    PLAYER_1,
    PLAYER_2
};

//---------------------------------------------------------------------
// Screen dimentions
//---------------------------------------------------------------------
enum {SCREEN_TOP = 0, SCREEN_BOTTOM = 192, SCREEN_LEFT = 0, SCREEN_RIGHT = 256};

//---------------------------------------------------------------------
// The number of frames per animation
//---------------------------------------------------------------------
#define FRAMES_PER_ANIMATION 4

#define ANIM_SPEED 4

#define SOUND_SPEED 8

#define PLAYER_SIZE 32

#define PLAYER_LOCAL_START_POS_X 25
#define PLAYER_LOCAL_START_POS_Y 34
//---------------------------------------------------------------------
// Initializing the player requires to load all of his graphics frames 
// into memory
//---------------------------------------------------------------------
void player_init(Player *player, u8* gfx);


//---------------------------------------------------------------------
// Updating the player requires to move him in the direction he is
// facing
//---------------------------------------------------------------------
void player_move(Player *player, int direction);


//---------------------------------------------------------------------
// Animating the player only requires to alter which sprite memory pointer
// he is using
//---------------------------------------------------------------------
void animate_player(Player *player);

void animate_remote_player(Player_remote *player);


void player_remote_init(Player_remote *player, u8* gfx, Data *remote_player);



