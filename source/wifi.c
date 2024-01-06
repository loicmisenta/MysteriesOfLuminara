#include "wifi.h"
#include <stdio.h>

#define DELAY 1000

void init_communication() {
	initWiFi();
	openSocket();
	iprintf("\x1b[0;0HCommunication initialized\n");
}

void init_data(Data *data) {
    data->player_number = 0;
    data->player_pos_x = 0;
    data->player_pos_y = 0;
	data->gfx_frame = 0;
}

void sendMessage(unsigned short keys, Data * local_player)
{
	char msg[5];

	//check if any arrow keys are pressed
	if(keys & KEY_UP || keys & KEY_DOWN || keys & KEY_LEFT || keys & KEY_RIGHT)
	{
		iprintf("\x1b[0;0HYou moved\n");
		//send the key that was pressed
		msg[0] = (char)(local_player->player_pos_x & 0xFF);
		msg[1] = (char)(local_player->player_pos_x >> 8) & 0xFF;
		msg[2] = (char)(local_player->player_pos_y & 0xFF);
		msg[3] = (char)(local_player->player_pos_y >> 8) & 0xFF;
		msg[4] = (char)(local_player->gfx_frame & 0x0F);



		sendData(msg, 5);
	}
}

void receiveMessage(Data * remote_player)
{
	char msg[5];
	int x, y = 0;
	int gfx_frame = 0;
	//Listen for messages from others
	if(receiveData(msg,5)>0){

		x = ((msg[1] <<8) | msg[0]);
		y = ((msg[3] <<8) | msg[2]);
		gfx_frame = msg[4];

		//If received, decode the position and print
		iprintf("\x1b[1;0HOther moved to %d, %d\n", x, y);
		remote_player->player_pos_x = x;
		remote_player->player_pos_y = y;
		remote_player->gfx_frame = gfx_frame;
	}
}
