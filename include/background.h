#include "powerUp.h"
#include "player.h"

typedef struct {
    int bg_size_min;
    int bg_size_max;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    int min_pos_x;
    int min_pos_y;
    int max_pos_x;
    int max_pos_y;
    int x;
    int y;
}Background;

enum BackgroundType{
    BACKGROUND_CLEAR,
    BACKGROUND_HIDDEN
};
#define WALL_OFFSET_BOTTOM 22
#define WALL_TILE 43

#define WIN_MIN_X 430
#define WIN_MIN_Y 405

void init_Background_struct(Background * background);

void update_Background_struct(Background * background, Player * player, int direction);

void init_main_BG_tiled();

void init_main_fog_tiled();

void init_main_BG_sprite();

void update_main_BG_tiled();

void update_main_fog_tiled();

void switch_bg();

int get_background_type();

void reset_sub();

void init_sub_BG();

void init_sub_start_BG();

void init_sub_start_BG_wifi();

void init_sub_end_pixelo_BG();

void init_sub_end_pixela_BG();

void init_sub_BG_sprite(PowerUp *powerUp);
//---------------------------------------------------------------------
// Updating the player's position on the map
//---------------------------------------------------------------------
void update_player_pos_on_BG(Player * player, Background * background);

u16 getTileColor(int x, int y);

#define POSX_PIXELLO_SELECT 33
#define POSY_PIXELLO_SELECT 62

#define POSX_PIXELLA_SELECT 160
#define POSY_PIXELLA_SELECT 62

void init_mainMenu();

void set_position_select_mainMenu(int x, int y, bool blink);

int collision_with_wall(Player * player);

int win_game(Player * local, Player_remote * remote);
