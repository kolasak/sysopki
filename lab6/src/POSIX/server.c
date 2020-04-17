#include "POSIX/chat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mqueue.h>

#include <unistd.h>


/* I dont feel like implementing almost identical thing twice */




int main()
{
	/* Analogically to system V chat example */

	/*
	 * Minimal example
	 */
	mqd_t server_mqd;
	mqd_t clients_mqd[CLIENT_CNT_MAX];



	server_mqd = chat_server_queue_init();

	struct chat_msg msg;
	unsigned int priority;


	chat_receive(server_mqd, &msg, CHAT_MSG_LEN_MAX, &priority);

	if (priority == start_t)
	{
		clients_mqd[0] = chat_client_queue_open(((struct chat_msg_start *) &msg)->name);
		((struct chat_msg_start *) &msg)->client_id = 0;

		chat_send(clients_mqd[0], &msg, CHAT_MSG_LEN_MAX,
				  start_t);     /* Here you would calculate length of the message */

		printf("Added client whose queue name is '%s'\n", ((struct chat_msg_start *) &msg)->name);
	} else
	{
		exit(EXIT_FAILURE);
	}


	chat_queue_destroy(SERVER_QUEUE_NAME);
	chat_queue_close(clients_mqd[0]);
	perror("");

	exit(EXIT_SUCCESS);
}