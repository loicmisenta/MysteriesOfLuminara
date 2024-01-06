#include <nds.h>
#include "timer_powerup.h"
#include "ISR.h"
#include "sounds.h"

int timer_ticks;
int timer_seconds;

int global_timer_ticks;
int global_timer_seconds;
int global_timer_milliseconds;

int TIME_TO_CHANGE_BG;

void timer_init(){
    timer_ticks = 0;
    timer_seconds = 0;
    TIME_TO_CHANGE_BG = TIMER_CHNG_BG;


    //configue timer 0 to generate an interrupt every 100ms
    TIMER0_DATA = TIMER_FREQ_1024(100);
    TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;

    irqSet(IRQ_TIMER0, &timer_ISR);
    irqEnable(IRQ_TIMER0);
}

void global_timer_init(){
    global_timer_ticks = 0;
    global_timer_seconds = 0;
    global_timer_milliseconds = 0;

    //configue timer 0 to generate an interrupt every 100ms
    TIMER2_DATA = TIMER_FREQ_1024(100);
    TIMER2_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;

    irqSet(IRQ_TIMER2, &global_timer_ISR);
    irqEnable(IRQ_TIMER2);
}

void global_timer_ISR(){
    global_timer_ticks++;
    if(global_timer_ticks >= 10){
        global_timer_ticks = 0;
        global_timer_milliseconds++;
    }
    if(global_timer_milliseconds >= 100){
        global_timer_milliseconds = 0;
        global_timer_seconds++;
    }
}

void timer_ISR(){
    timer_ticks++;
    if(timer_ticks >= 100){
        timer_ticks = 0;
        timer_seconds++;
    }
    if(timer_seconds >= TIMER_POWER_UP){
        powerUp_ISR();
    }

    if(timer_seconds >= TIME_TO_CHANGE_BG){
        changeBG_ISR();
    }else{
        if(timer_ticks % 20 == 0){
            if(timer_ticks != 0 || timer_seconds != 0){
                play_timer();
            }
        }
    }
}

void timer_start(){
    irqEnable(IRQ_TIMER0);
}

void global_timer_start(){
    irqEnable(IRQ_TIMER2);
}

void timer_stop(){
    irqDisable(IRQ_TIMER0);
}

void global_timer_stop(){
    irqDisable(IRQ_TIMER2);
}

void timer_reset(){
    timer_ticks = 0;
    timer_seconds = 0;
    //change the time to change the background to a random number between 2 and 6
    TIME_TO_CHANGE_BG = (int) (rand() % 2 + 1);
}

void global_timer_reset(){
    global_timer_ticks = 0;
    global_timer_seconds = 0;
    global_timer_milliseconds = 0;
}

int timer_get_seconds(){
    return timer_seconds;
}

int global_timer_get_seconds(){
    return global_timer_seconds;
}

int global_timer_get_milliseconds(){
    return global_timer_milliseconds;
}