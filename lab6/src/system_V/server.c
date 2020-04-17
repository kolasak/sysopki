#include "system_V/chat.h"
#include "system_V/server_routines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int server_qid;
int clients_qid[CLIENT_CNT_MAX] = {[0 ... CLIENT_CNT_MAX - 1] = -1};


static void server_run(void);



int main(int argc, char **argv)
{
	server_setup();

	server_run();

	exit(EXIT_SUCCESS);
}



void server_run(void)
{
	struct chat_msg message;
	size_t message_data_size;


	while ( 1 )
	{
		message_data_size = chat_receive(server_qid, &message, CHAT_MSG_DATA_SIZE_MAX, -999999);
		if (message_data_size == -1)
		{
			perror("Failed to receive message");
			exit(EXIT_FAILURE);
		}


		int retval;
		switch (message.type)
		{
			case stop_t:
				retval = server_client_stop_handle((struct chat_msg_stop *) &message, message_data_size);
				break;

			case start_t:
				retval = server_client_start_handle((struct chat_msg_start *) &message, message_data_size);
				break;

			case unicast_t:
				retval = server_client_unicast_handle((struct chat_msg_unicast *) &message, message_data_size);
				break;

			case foo_t:
			default:
				retval = -1;
				/* ... */
				break;
		}
		if (retval)
			printf("\nFailed to process message of %lx type\n", message.type);


	}

}


