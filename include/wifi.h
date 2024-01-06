#include "WiFi_minilib.h"
#include <stdlib.h>
#include <time.h>
#include "ISR.h"

enum Locality{
    LOCAL,
    REMOTE
};

//#pragma pack(1)
typedef struct {
    int unique_id;
    int player_number;
    int player_pos_x;
    int player_pos_y;
    int gfx_frame;
}Data;

#define MAX_MESSAGE_SIZE 9

void init_communication();

void init_data(Data * data);

void sendMessage(unsigned short keys, Data * local_player);

void receiveMessage(Data * remote_player);