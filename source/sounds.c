#include "sounds.h"

void init_sound(){
    mmInitDefaultMem((mm_addr)soundbank_bin);

    mmLoad(MOD_BACKGROUND_MUSIC);

    mmLoadEffect(SFX_CLICK);

    mmLoadEffect(SFX_DOOR);

    mmLoadEffect(SFX_TIMER);

    mmLoadEffect(SFX_POWERUP);

    mmLoadEffect(SFX_FOOTSTEPS);

    mmLoadEffect(SFX_VICTORY);

    mmLoadEffect(SFX_OBJECT_ACTIVATION);

    mmLoadEffect(SFX_WALL);
}

void play_background_music(){
    mmStart(MOD_BACKGROUND_MUSIC, MM_PLAY_LOOP);
    mmSetModuleVolume(100); //0-1024
}

void play_powerup(){
    mmEffect(SFX_POWERUP);
}

void play_footsteps(){
    mmEffect(SFX_FOOTSTEPS);
}

void play_wall(){
    mmEffect(SFX_WALL);
}

void play_click(){
    mmEffect(SFX_CLICK);
}

void play_timer(){
    mmEffectVolume(mmEffect(SFX_TIMER), 70);
}

void play_door(){
    mmEffect(SFX_DOOR);
}

void play_object_activation(){
    mmEffect(SFX_OBJECT_ACTIVATION);
}

void play_victory(){
    mmEffect(SFX_VICTORY);
}