#include <nds.h>
#include "background.h"
#include "BG.h"
#include "fog.h"
#include "sub_screen.h"
#include "powerup_pixelo_sprite.h"
#include "powerup_bw_pixelo_sprite.h"
#include "sounds.h"
#include "chooseplayer.h"
#include "select_mainMenu.h"
#include "sub_screen_start.h"
#include "sub_screen_start_connected.h"
#include "pixelo_win.h"
#include "pixela_win.h"

int background_type = BACKGROUND_CLEAR;

void init_main_BG_tiled(){
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
    REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;
    BGCTRL[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_64x64;
}

void init_main_fog_tiled(){
    BGCTRL[1] = BG_COLOR_256 | BG_MAP_BASE(4) | BG_TILE_BASE(2) | BG_64x64;
}

void init_main_BG_sprite(){
    VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE;
    oamInit(&oamMain, SpriteMapping_1D_128, false);

    // add vram for the remote player
    VRAM_G_CR = VRAM_ENABLE | VRAM_G_MAIN_SPRITE;
    oamInit(&oamMain, SpriteMapping_1D_128, false);
}

void init_Background_struct(Background * background){
    background->min_x = 0;
    background->min_y = 0;
    background->max_x = 512 - 256;
    background->max_y = 512 - 192;
    background->bg_size_min = 8;
    background->bg_size_max = 504;
    background->min_pos_x = 256/2 - 16;
    background->min_pos_y = 192/2 - 16;
    background->max_pos_x = 512 - 256/2 - 16;
    background->max_pos_y = 512 - 192/2 - 16;
    background->x = 0;
    background->y = 0;
}

void update_player_pos_on_BG(Player * player, Background * background){
    player->player_pos_x = background->x + player->x;
    player->player_pos_y = background->y + player->y;
}

void update_BG_references(Background * background){
    REG_BG0VOFS = background->y;
    REG_BG0HOFS = background->x;
    REG_BG1VOFS = background->y;
    REG_BG1HOFS = background->x;
}

void update_Background_struct(Background * background, Player * player, int direction){
    if(collision_with_wall(player)){
        play_wall();
        player->x = PLAYER_LOCAL_START_POS_X;
        player->y = PLAYER_LOCAL_START_POS_Y;
        player->direction = PLAYER_DIRECTION_DOWN;
        background->x = 0;
        background->y = 0;
        update_player_pos_on_BG(player, background);
        update_BG_references(background);
        return;
    }
    if(direction == PLAYER_DIRECTION_UP){
        if(player->player_pos_y < background->bg_size_min){
            play_wall();
            return;
        }
        background->y -= player->speed;
        update_player_pos_on_BG(player, background);
        if(player->player_pos_y < background->min_pos_y){
            background->y = background->min_y;
            player_move(player, direction);
        }
        else if(player->player_pos_y > background->max_pos_y){
            background->y = background->max_y;
            player_move(player, direction);
        }
        else{
            update_BG_references(background);
        }
    }
    if(direction == PLAYER_DIRECTION_DOWN){
        if(player->player_pos_y > background->bg_size_max - WALL_OFFSET_BOTTOM - PLAYER_SIZE){
            play_wall();
            return;
        }
        background->y += player->speed;
        update_player_pos_on_BG(player, background);
        if(player->player_pos_y < background->min_pos_y){
            background->y = background->min_y;
            player_move(player, direction);
        }
        else if(player->player_pos_y > background->max_pos_y){
            background->y = background->max_y;
            player_move(player, direction);
        }
        else{
            update_BG_references(background);
        }
    }
    if(direction == PLAYER_DIRECTION_LEFT){
        if(player->player_pos_x < background->bg_size_min){
            play_wall();
            return;
        }
        background->x -= player->speed;
        update_player_pos_on_BG(player, background);
        if(player->player_pos_x < background->min_pos_x){
            background->x = background->min_x;
            player_move(player, direction);
        }
        else if(player->player_pos_x > background->max_pos_x){
            background->x = background->max_x;
            player_move(player, direction);
        }
        else{
            update_BG_references(background);
        }
    }
    if(direction == PLAYER_DIRECTION_RIGHT){
        if(player->player_pos_x > background->bg_size_max - PLAYER_SIZE){
            play_wall();
            return;
        }
        background->x += player->speed;
        update_player_pos_on_BG(player, background);
        if(player->player_pos_x < background->min_pos_x){
            background->x = background->min_x;
            player_move(player, direction);
        }
        else if(player->player_pos_x > background->max_pos_x){
            background->x = background->max_x;
            player_move(player, direction);
        }
        else{
            update_BG_references(background);
        }
    }
}

void update_main_BG_tiled(){
    dmaCopy(BGTiles, BG_TILE_RAM(1), BGTilesLen);
    dmaCopy(BGPal, BG_PALETTE, BGPalLen);
    int i = 0;
    for(i=0; i<32; i++)
		dmaCopy(&BGMap[i*64], &((uint16*)(BG_MAP_RAM(0)))[i*32], 64);

	// TOP RIGHT quadrant of the image in second map base
	for(i=0; i<32; i++)
		dmaCopy(&BGMap[i*64+32], &((uint16*)(BG_MAP_RAM(1)))[i*32], 64);

	// BOTTOM LEFT quadrant of the image in third map base
	for(i=0; i<32; i++)
		dmaCopy(&BGMap[(i+32)*64], &((uint16*)(BG_MAP_RAM(2)))[i*32], 64);

	// BOTTOM RIGHT quadrant of the image in fourth map base
	for(i=0; i<32; i++)
		dmaCopy(&BGMap[(i+32)*64+32], &((uint16*)(BG_MAP_RAM(3)))[i*32], 64);
    background_type = BACKGROUND_CLEAR;
    
}

void update_main_fog_tiled(){
    dmaCopy(fogTiles, BG_TILE_RAM(2), fogTilesLen);
    // dmaCopy(fogPal, BG_PALETTE, fogPalLen);
    int i = 0;
    for(i=0; i<32; i++)
		dmaCopy(&fogMap[i*64], &BG_MAP_RAM(4)[i*32], 64);

	// TOP RIGHT quadrant of the image in second map base
	for(i=0; i<32; i++)
		dmaCopy(&fogMap[i*64+32], &BG_MAP_RAM(5)[i*32], 64);

	// BOTTOM LEFT quadrant of the image in third map base
	for(i=0; i<32; i++)
		dmaCopy(&fogMap[(i+32)*64], &BG_MAP_RAM(6)[i*32], 64);

	// BOTTOM RIGHT quadrant of the image in fourth map base
	for(i=0; i<32; i++)
		dmaCopy(&fogMap[(i+32)*64+32], &BG_MAP_RAM(7)[i*32], 64);
}

void switch_bg(){
    if(background_type == BACKGROUND_CLEAR){
        dmaCopy(fogPal, BG_PALETTE, fogPalLen);
        background_type = BACKGROUND_HIDDEN;
    }
    else{
        dmaCopy(BGPal, BG_PALETTE, BGPalLen);
        background_type = BACKGROUND_CLEAR;
    }
    REG_DISPCNT ^= DISPLAY_BG0_ACTIVE;
    REG_DISPCNT ^= DISPLAY_BG1_ACTIVE;
}

int get_background_type(){
    return background_type;
}

void reset_sub(){
    oamClear(&oamSub, 0, 0);
    REG_DISPCNT_SUB = 0;
    VRAM_C_CR = 0;
    VRAM_I_CR = 0;
}

void init_sub_start_BG(){
    REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
    BGCTRL_SUB[2] = BG_BMP_BASE(0) | BG_BMP8_256x256;
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;
    dmaCopy(sub_screen_startBitmap, BG_GFX_SUB, sub_screen_startBitmapLen);
    dmaCopy(sub_screen_startPal, BG_PALETTE_SUB, sub_screen_startPalLen);
}

void init_sub_start_BG_wifi(){
    REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
    BGCTRL_SUB[2] = BG_BMP_BASE(0) | BG_BMP8_256x256;
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;
    dmaCopy(sub_screen_start_connectedBitmap, BG_GFX_SUB, sub_screen_start_connectedBitmapLen);
    dmaCopy(sub_screen_start_connectedPal, BG_PALETTE_SUB, sub_screen_start_connectedPalLen);
}

void init_sub_BG(){
    reset_sub();
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
    VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	BGCTRL_SUB[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_32x32;
	dmaCopy(sub_screenTiles, BG_TILE_RAM_SUB(1), sub_screenTilesLen);
	dmaCopy(sub_screenPal, BG_PALETTE_SUB, sub_screenPalLen);
	dmaCopy(sub_screenMap, BG_MAP_RAM_SUB(0), sub_screenMapLen);
}

void init_sub_end_pixelo_BG(){
    reset_sub();
    REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE;
    VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
    BGCTRL_SUB[2] = BG_BMP_BASE(0) | BG_BMP8_256x256;
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;
    dmaCopy(pixelo_winBitmap, BG_GFX_SUB, pixelo_winBitmapLen);
    dmaCopy(pixelo_winPal, BG_PALETTE_SUB, pixelo_winPalLen);
}

void init_sub_end_pixela_BG(){
    reset_sub();
    REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE;
    VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
    BGCTRL_SUB[2] = BG_BMP_BASE(0) | BG_BMP8_256x256;
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;
    dmaCopy(pixela_winBitmap, BG_GFX_SUB, pixela_winBitmapLen);
    dmaCopy(pixela_winPal, BG_PALETTE_SUB, pixela_winPalLen);
}

void init_sub_BG_sprite(PowerUp *powerUp){
	VRAM_I_CR = VRAM_ENABLE | VRAM_I_SUB_SPRITE;
	oamInit(&oamSub, SpriteMapping_1D_128, false);
    powerUp->bw_sprite_gfx_mem = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_Bmp);
    powerUp->color_sprite_gfx_mem = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_256Color);
}

u16 getTileColor(int x, int y) {
    // Calculate tile coordinates
    int tileX = x / 8;
    int tileY = y / 8;

    u16 tileValue;
    if(tileX < 32 && tileY < 32){
        tileValue = BG_MAP_RAM(0)[(tileY % 32) * 32 + (tileX % 32)];
    }
    else if(tileX >= 32 && tileY < 32){
        tileValue = BG_MAP_RAM(1)[(tileY % 32) * 32 + (tileX % 32)];
    }
    else if(tileX < 32 && tileY >= 32){
        tileValue = BG_MAP_RAM(2)[(tileY % 32) * 32 + (tileX % 32)];
    }
    else if(tileX >= 32 && tileY >= 32){
        tileValue = BG_MAP_RAM(3)[(tileY % 32) * 32 + (tileX % 32)];
    }

    return tileValue;
}

int collision_with_wall(Player * player){
    u16 tileValue;
    if(player->direction == PLAYER_DIRECTION_UP){
        tileValue = getTileColor(player->player_pos_x + player->playerCollision.top_collide_x, player->player_pos_y + player->playerCollision.top_collide_y);
    }else if(player->direction == PLAYER_DIRECTION_DOWN){
        tileValue = getTileColor(player->player_pos_x + player->playerCollision.bottom_collide_x, player->player_pos_y + player->playerCollision.bottom_collide_y);
    }else if(player->direction == PLAYER_DIRECTION_LEFT){
        tileValue = getTileColor(player->player_pos_x + player->playerCollision.left_collide_x, player->player_pos_y + player->playerCollision.left_collide_y);
    }else if(player->direction == PLAYER_DIRECTION_RIGHT){
        tileValue = getTileColor(player->player_pos_x + player->playerCollision.right_collide_x, player->player_pos_y + player->playerCollision.right_collide_y);
    }
    if(tileValue == WALL_TILE){
        return 1;
    }
    return 0;
}

int win_game(Player * local, Player_remote * remote){
    if(local->player_pos_x >= WIN_MIN_X && local->player_pos_y >= WIN_MIN_Y){
        reset_sub();
        switch_bg();
        local->player_pos_x = WIN_MIN_X + 2;
        local->player_pos_y = WIN_MIN_Y + 2;
        return PLAYER_1;
    }
    if(remote->player_pos_x >= WIN_MIN_X && remote->player_pos_y >= WIN_MIN_Y){
        reset_sub();
        switch_bg();
        remote->player_pos_x = WIN_MIN_X + 2;
        remote->player_pos_y = WIN_MIN_Y + 2;
        return PLAYER_2;
    }
    return -1;
}

//---------------------------------------------------------------------
// MAIN MENU
//---------------------------------------------------------------------

void init_mainMenu(){
    //DO 2 BG because we want to have the select_mainMenu on top of the chooseplayer
    REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
    BGCTRL[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_64x64;
    dmaCopy(chooseplayerTiles, BG_TILE_RAM(1), chooseplayerTilesLen);
    dmaCopy(chooseplayerPal, BG_PALETTE, chooseplayerPalLen);
    dmaCopy(chooseplayerMap, BG_MAP_RAM(0), chooseplayerMapLen);

    //put the sprite select on top of it
    VRAM_G_CR = VRAM_ENABLE | VRAM_G_MAIN_SPRITE;
	oamInit(&oamMain, SpriteMapping_1D_128, false);
    oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);
}

void set_position_select_mainMenu(int x, int y, bool blink){
    dmaCopy(select_mainMenuPal, SPRITE_PALETTE, select_mainMenuPalLen);
    dmaCopy(select_mainMenuTiles, SPRITE_GFX, select_mainMenuTilesLen);
    oamSet(&oamMain, 0, x, y, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, SPRITE_GFX, -1, false, blink, false, false, false);
}