//---------------------------------------------------------------------
// Timer 
//---------------------------------------------------------------------


void global_timer_init();

void global_timer_ISR();

void global_timer_start();

void global_timer_stop();

void global_timer_reset();

int global_timer_get_seconds();

int global_timer_get_milliseconds();

void timer_init();

void timer_start();

void timer_stop();

void timer_reset();

void timer_ISR();

int timer_get_seconds();

#define TIMER_POWER_UP 5

#define TIMER_CHNG_BG 3