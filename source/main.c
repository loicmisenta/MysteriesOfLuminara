#include <nds.h>
#include "background.h"
#include "player_sprite.h"
#include "player2_sprite.h"
#include "sounds.h"
#include "timer_powerup.h"
#include "ISR.h"
#include <stdio.h>
#include <stdlib.h>

PowerUp powerUp;
Data remote_player; 
Data local_player;
int ID;
Player_remote remote_player_struct;

// Define our custom font color 
int red = 31 * 252 / 255;       // Approximate to 5 bits
int green = 31 * 236 / 255;     // Approximate to 5 bits
int blue = 31 * 209 / 255;      // Approximate to 5 bits

enum GameState{
    MAIN_MENU,
    PLAYING,
    WIN
};

void powerUp_ISR(){
    if (!powerUp.active){
        enable(&powerUp, &oamSub);
        play_powerup();
        timer_stop();
        timer_reset();
    }
}

void changeBG_ISR(){
    if(get_background_type() == BACKGROUND_CLEAR){
        switch_bg();
    }

}

int main(void) {

    consoleDemoInit();
    Background background;
    init_Background_struct(&background);
    
    enum GameState gameState = MAIN_MENU;
    
    Player player;

    int winner_id;
	
    init_data(&remote_player);
    init_data(&local_player);

    init_sound();
        
    player.player_pos_x = PLAYER_LOCAL_START_POS_X;
    player.player_pos_y = PLAYER_LOCAL_START_POS_Y;
    remote_player_struct.player_pos_x = PLAYER_LOCAL_START_POS_X;
    remote_player_struct.player_pos_y = PLAYER_LOCAL_START_POS_Y;

    int menuDisplayed = 0;
    int wifi_init = 0;

    int player_number = PLAYER_1;
    int player_number_selected = 0;

    int i = 0;
    int count_blink = 0;
    bool blink = false;
    while(1) {    
       
        if(gameState == MAIN_MENU){
            scanKeys();
            int keys = keysDown(); 
            //---------------------------------------------------------------------------------
            // Display Main Menu
            //---------------------------------------------------------------------------------
            if(!menuDisplayed){
                init_mainMenu();
                init_sub_start_BG();
                menuDisplayed = 1;
            }
            if(player_number == PLAYER_1){
                if(player_number_selected == 0){
                    if(count_blink%15 == 0){
                        blink = !blink;
                    }
                    set_position_select_mainMenu(POSX_PIXELLO_SELECT, POSY_PIXELLO_SELECT, blink);
                }
                else if(player_number_selected == 1){
                    set_position_select_mainMenu(POSX_PIXELLO_SELECT, POSY_PIXELLO_SELECT, false);
                    local_player.player_number = PLAYER_1;
                }
                
            }
            if(player_number == PLAYER_2){
                if(player_number_selected == 0){
                    if(count_blink%15 == 0){
                        blink = !blink;
                    }
                    set_position_select_mainMenu(POSX_PIXELLA_SELECT, POSY_PIXELLA_SELECT, blink);
                }
                else if(player_number_selected == 1){
                    set_position_select_mainMenu(POSX_PIXELLA_SELECT, POSY_PIXELLA_SELECT, false);
                    local_player.player_number = PLAYER_2;
                }
            }

            if(keys & KEY_A){
                play_click();
                if(player_number_selected == 0){
                    player_number_selected = 1;
                }
                else if(player_number_selected == 1){}
            }
            if(keys & KEY_RIGHT){
                play_click();
                player_number = PLAYER_2;
                if(player_number_selected == 1){
                    player_number_selected = 0;
                }
            }
            if(keys & KEY_LEFT){
                play_click();
                player_number = PLAYER_1;
                if(player_number_selected == 2){
                    player_number_selected = 0;
                }
            }
            
            if(keys & KEY_SELECT){
                play_click();
                if(!wifi_init){
                    init_communication();
                    wifi_init = 1;
                    init_sub_start_BG_wifi();
                    iprintf("connected\n");
                }
            }

            if(keys & KEY_START && player_number_selected == 1){
                play_click();
                //---------------------------------------------------------------------------------
                // GAME AS STARTED
                // Initialize variables, display, and sound
                //---------------------------------------------------------------------------------

                initPowerUp(&powerUp, POWERUP_PIXELO);
                
                //---------------------------------------------------------------------------------
                // Setup the Main screen for 2D graphics
                //---------------------------------------------------------------------------------
                init_main_BG_tiled();
                init_main_fog_tiled();

                update_main_BG_tiled();
                update_main_fog_tiled();

                init_main_BG_sprite();
                reset_sub();
                init_sub_BG();
                init_sub_BG_sprite(&powerUp);

                timer_init();
                timer_start();

                global_timer_init();
                global_timer_start();
                    
                // disable at start
                disable(&powerUp, &oamSub);
                play_background_music();

                //---------------------------------------------------------------------------------
                // Initialize sprites
                //---------------------------------------------------------------------------------
                if(player_number == PLAYER_2){
                    player_init(&player, (u8*)player_spriteTiles);
                    dmaCopy(player_spritePal, SPRITE_PALETTE, player_spritePalLen);

                    //init player remote
                    player_remote_init(&remote_player_struct, (u8*)player2_spriteTiles, &remote_player);
                    dmaCopy(player2_spritePal, SPRITE_PALETTE_SUB, player2_spritePalLen);

                }
                else if(player_number == PLAYER_1){
                    player_init(&player, (u8*)player2_spriteTiles);
                    dmaCopy(player2_spritePal, SPRITE_PALETTE, player2_spritePalLen);

                    //init player remote
                    player_remote_init(&remote_player_struct, (u8*)player_spriteTiles, &remote_player);
                    dmaCopy(player_spritePal, SPRITE_PALETTE_SUB, player_spritePalLen);

                }

                gameState = PLAYING;
            }
            else if(keys & KEY_B){
                //---------------------------------------------------------------------------------
                // EXIT GAME
                //---------------------------------------------------------------------------------
                return 0;
            }
            count_blink++;
            oamUpdate(&oamMain);
            swiWaitForVBlank();
            
        }
        else if(gameState == PLAYING){

            scanKeys();
            int keys = keysHeld(); 

            winner_id = win_game(&player, &remote_player_struct);
            if(winner_id >= 0){
                play_victory();
                gameState=WIN;
            }
            //---------------------------------------------------------------------------------
            // GAME IS PLAYING
            // Main loop
            //---------------------------------------------------------------------------------
            local_player.player_pos_x = player.player_pos_x;
            local_player.player_pos_y = player.player_pos_y;
            local_player.unique_id = ID;

            sendMessage(keys, &local_player);
            receiveMessage(&remote_player);

            //---------------------------------------------------------------------------------
            // Update player remote struct
            //---------------------------------------------------------------------------------

            remote_player_struct.player_pos_x = remote_player.player_pos_x;
            remote_player_struct.player_pos_y = remote_player.player_pos_y;
            remote_player_struct.gfx_frame = remote_player.gfx_frame;
            local_player.gfx_frame = player.gfx_frame;

            iprintf("\x1b[0;0Hgfx_frame: %d", remote_player_struct.gfx_frame);
            //move
            //---------------------------------------------------------------------------------

            if(keys){
                
                if(keys & KEY_RIGHT) {
                    player.direction = PLAYER_DIRECTION_RIGHT;
                    update_Background_struct(&background, &player, player.direction);
                }
                else if(keys & KEY_DOWN) {
                    player.direction = PLAYER_DIRECTION_DOWN;
                    update_Background_struct(&background, &player, player.direction);
                }
                else if(keys & KEY_LEFT) {
                    player.direction = PLAYER_DIRECTION_LEFT;
                    update_Background_struct(&background, &player, player.direction);
                }
                else if(keys & KEY_UP) {
                    player.direction = PLAYER_DIRECTION_UP;
                    update_Background_struct(&background, &player, player.direction);
                }
                
                if(keys & KEY_RIGHT || keys & KEY_LEFT || keys & KEY_UP || keys & KEY_DOWN){
                    i++;
                    if(i % ANIM_SPEED == 0){
                        player.anim_frame++;
                    }
                    if(i % SOUND_SPEED == 0){
                        play_footsteps();
                    }
                }

                if(player.anim_frame >= FRAMES_PER_ANIMATION) {
                    player.anim_frame = 0;
                }                
            }
            touchPosition touch;
            touchRead(&touch);
            //---------------------------------------------------------------------------------
            // POWERUP
            //---------------------------------------------------------------------------------
            //after 5 sec enable powerup

            if(touch.px > POWERUP_POSITION_X && touch.px < POWERUP_POSITION_X + 64 && touch.py > POWERUP_POSITION_Y && touch.py < POWERUP_POSITION_Y + 64){
                if(powerUp.active){
                    switch_bg();
                    disable(&powerUp, &oamSub);
                    play_click();
                    timer_start();
                }
            }

            animate_player(&player);

            iprintf("\x1b[3;0H bgX:%d bgY:%d\n", background.x, background.y);
            iprintf("\x1b[4;0H hidden:%d\n", remote_player_struct.is_hidden);
            int min_view_x = background.x - 32;
            int max_view_x = background.x + 256;
            int min_view_y = background.y - 32;
            int max_view_y = background.y + 192;

            if(remote_player_struct.player_pos_x > min_view_x && remote_player_struct.player_pos_x < max_view_x && remote_player_struct.player_pos_y > min_view_y && remote_player_struct.player_pos_y < max_view_y){
                remote_player_struct.is_hidden = 0;
                remote_player_struct.x = remote_player_struct.player_pos_x - background.x;
                remote_player_struct.y = remote_player_struct.player_pos_y - background.y;
            }
            else{
                remote_player_struct.is_hidden = 1;
            }
            //---------------------------------------------------------------------------------
            // Update OAM
            //---------------------------------------------------------------------------------
            
            oamSet(&oamMain, //main graphics engine context
                0,           //oam index (0 to 127)  
                player.x, player.y,   //x and y pixle location of the sprite
                0,                    //priority, lower renders last (on top)
                0,                    //this is the palette index if multiple palettes or the alpha value if bmp sprite 
                SpriteSize_32x32,     
                SpriteColorFormat_256Color, 
                player.sprite_gfx_mem[player.gfx_frame],                  //pointer to the loaded graphics
                -1,                  //sprite rotation data  
                false,               //double the size when rotating?
                false,               //hide the sprite?
                false, false,        //vflip, hflip
                false                //apply mosaic
                );
            
            //update remote player
            
            oamSet(&oamMain, //main graphics engine context
                4,           //oam index (0 to 127)  has to be a multiple of 4 
                remote_player_struct.x, remote_player_struct.y,   //x and y pixle location of the sprite
                0,                    //priority, lower renders last (on top)
                0,                    //this is the palette index if multiple palettes or the alpha value if bmp sprite 
                SpriteSize_32x32,     
                SpriteColorFormat_256Color, 
                remote_player_struct.sprite_gfx_mem[remote_player_struct.gfx_frame],                  //pointer to the loaded graphics
                -1,                  //sprite rotation data  
                false,               //double the size when rotating?
                remote_player_struct.is_hidden,               //hide the sprite?
                false, false,        //vflip, hflip
                false                //apply mosaic
                );

            //---------------------------------------------------------------------------------
            // print the global timer on the screen
            //---------------------------------------------------------------------------------

            //global with miliseconds
            int sec = global_timer_get_seconds();
            int milisec = global_timer_get_milliseconds();
            //print timer on the sub screen
            iprintf("\x1b[15;8HGlobal timer: %d.%d", sec, milisec);
            
            swiWaitForVBlank();
            oamUpdate(&oamMain);
            oamUpdate(&oamSub);
        }
        else if(gameState == WIN){
            player_number == winner_id ? init_sub_end_pixelo_BG() : init_sub_end_pixela_BG();
            swiWaitForVBlank();        
        }
    }
    return 0;

}