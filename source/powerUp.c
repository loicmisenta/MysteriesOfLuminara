#include "powerUp.h"
#include "powerup_pixelo_sprite.h"
#include "powerup_bw_pixelo_sprite.h"



void initPowerUp(PowerUp *powerUp, int type){
    powerUp->active = 0;
    powerUp->type = type;
}

void displayPowerUp(PowerUp *powerUp, OamState *oam){
    // Clear the OAM entry for the power-up sprite
	if(powerUp->type == POWERUP_PIXELO){}

    if(powerUp->active){
        // Display color sprite
        dmaCopy(powerup_pixelo_spritePal, SPRITE_PALETTE_SUB, powerup_pixelo_spritePalLen);
        dmaCopy(powerup_pixelo_spriteTiles, powerUp->color_sprite_gfx_mem, powerup_pixelo_spriteTilesLen);
        oamSet(oam, 0, POWERUP_POSITION_X, POWERUP_POSITION_Y, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, powerUp->color_sprite_gfx_mem, -1, false, false, false, false, false);
    }
    else if (!powerUp->active){
        // Display black and white sprite
        dmaCopy(powerup_bw_pixelo_spritePal, SPRITE_PALETTE_SUB, powerup_bw_pixelo_spritePalLen);
        dmaCopy(powerup_bw_pixelo_spriteTiles, powerUp->bw_sprite_gfx_mem, powerup_bw_pixelo_spriteTilesLen);
        oamSet(oam, 0, POWERUP_POSITION_X, POWERUP_POSITION_Y, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, powerUp->bw_sprite_gfx_mem, -1, false, false, false, false, false);

    }
}

void activatePowerUp(PowerUp *powerUp){
    powerUp->active = 1;
}

void deactivatePowerUp(PowerUp *powerUp){
    powerUp->active = 0;

}

void enable(PowerUp *powerUp, OamState *oam){
    activatePowerUp(powerUp);
    displayPowerUp(powerUp, oam);
}

void disable(PowerUp *powerUp, OamState *oam){
    deactivatePowerUp(powerUp);
    displayPowerUp(powerUp, oam);
}


