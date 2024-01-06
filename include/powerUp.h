#include <nds.h>

//---------------------------------------------------------------------
// PowerUp struct definition
//---------------------------------------------------------------------

typedef struct
{
    int active;
    int type;
    void* color_sprite_gfx_mem;
    void* bw_sprite_gfx_mem;
    
} PowerUp;

//---------------------------------------------------------------------
// Enum Type 
//---------------------------------------------------------------------

enum PowerUpType
{
    POWERUP_PIXELO,
    POWERUP_PIXELLA

};

//---------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------
#define POWERUP_POSITION_X 156
#define POWERUP_POSITION_Y 21

//---------------------------------------------------------------------
// Function Prototypes
//---------------------------------------------------------------------

void initPowerUp(PowerUp *powerUp, int type);

void displayPowerUp(PowerUp *powerUp, OamState *oam);

void activatePowerUp(PowerUp *powerUp);

void deactivatePowerUp(PowerUp *powerUp);

void enable(PowerUp *powerUp, OamState *oam);

void disable(PowerUp *powerUp, OamState *oam);
