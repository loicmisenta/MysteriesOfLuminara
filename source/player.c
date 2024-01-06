#include <nds.h>
#include "player.h"


void player_init(Player *player, u8* gfx){
    player->x = PLAYER_LOCAL_START_POS_X;
    player->y = PLAYER_LOCAL_START_POS_Y;
    player->direction = PLAYER_DIRECTION_DOWN;
    player->speed = 1;
    player->anim_frame = 0;

    int i;

	for(i = 0; i < 16; i++)
	{
		player->sprite_gfx_mem[i] = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
		dmaCopy(gfx, player->sprite_gfx_mem[i], 32*32);
		gfx += 32*32;
	}
    player->playerCollision.right_collide_x = 22;
    player->playerCollision.right_collide_y = 24;
    player->playerCollision.left_collide_x = 9;
    player->playerCollision.left_collide_y = 24;
    player->playerCollision.top_collide_x = 16;
    player->playerCollision.top_collide_y = 20;
    player->playerCollision.bottom_collide_x = 16;
    player->playerCollision.bottom_collide_y = 28;
}

void player_move(Player *player, int direction) {
    int x = player->x;
    int y = player->y;
    int speed = player->speed;

    switch(direction) {
        case PLAYER_DIRECTION_RIGHT:
            x += speed;
            break;
        case PLAYER_DIRECTION_DOWN:
            y += speed;
            break;
        case PLAYER_DIRECTION_LEFT:
            x -= speed;
            break;
        case PLAYER_DIRECTION_UP:
            y -= speed;
            break;
    }
    player->direction = direction;
    player->x = x;
    player->y = y;
}

void animate_player(Player *player){
    player->gfx_frame = player->anim_frame + player->direction * FRAMES_PER_ANIMATION;
}

//---------------------------------------------------------------------
// Player remote 
//---------------------------------------------------------------------

void player_remote_init(Player_remote *player, u8* gfx, Data *remote_player){
    player->x = PLAYER_LOCAL_START_POS_X;
    player->y = PLAYER_LOCAL_START_POS_Y;
    remote_player->player_pos_x = PLAYER_LOCAL_START_POS_X;
    remote_player->player_pos_y = PLAYER_LOCAL_START_POS_Y;
    player->direction = PLAYER_DIRECTION_DOWN;
    player->speed = 1;
    player->anim_frame = 0;
    player->is_hidden = 0;

    int i;

    for(i = 0; i < 16; i++)
    {
        player->sprite_gfx_mem[i] = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
        dmaCopy(gfx, player->sprite_gfx_mem[i], 32*32);
        gfx += 32*32;
    }
}

void animate_remote_player(Player_remote *player){
    player->gfx_frame = player->anim_frame + player->direction * FRAMES_PER_ANIMATION;
}

