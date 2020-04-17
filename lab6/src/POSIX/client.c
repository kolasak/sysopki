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
	char client_queue_name[80];
	mqd_t server_mqd;
	mqd_t client_mqd;



	sprintf(client_queue_name, "/client_POSIX_%d", getpid());
	client_mqd = chat_client_queue_init(client_queue_name);
	server_mqd = chat_server_queue_open();


	static struct chat_msg_start msg_start = {
			.client_id = -1,
	};
	strcpy(msg_start.name, client_queue_name);

	chat_send(server_mqd, &msg_start, CHAT_MSG_LEN_MAX,
			  start_t);    /* Here you would calculate length of the message */


	unsigned int prio;
	chat_receive(client_mqd, &msg_start, CHAT_MSG_LEN_MAX, &prio);

	if (prio != start_t)
		exit(EXIT_FAILURE);

	printf("Registered as client %d\n", msg_start.client_id);



	chat_queue_destroy(client_queue_name);
	chat_queue_close(server_mqd);
	perror("");

	exit(EXIT_SUCCESS);
}