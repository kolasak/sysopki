#include "system_V/chat.h"
#include "system_V/client_routines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>



extern int server_qid;
extern int client_qid;
extern int client_id;



static void signal_handler(int signum)
{
	exit(EXIT_SUCCESS);
}

void client_setup(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);


	/*
	 * Obtain id of server's and client's message queue
	 */
	printf("Opening server's message queue id... ");
	server_qid = chat_server_queue_open();
	if (server_qid == -1)
	{
		perror("Couldn't open server's message queue");
		exit(EXIT_FAILURE);
	}
	printf("[OK] - qid: %d\n", server_qid);


	printf("Creating client's message queue id... ");
	client_qid = chat_client_queue_init();
	if (client_qid == -1)
	{
		perror("Couldn't create client's message queue");
		exit(EXIT_FAILURE);
	}
	printf("[OK] - qid: %d\n", client_qid);


	atexit(on_client_stop);
	signal(SIGINT, signal_handler);


	/*
	 * Register with server
	 */
	printf("Sending START message to server... ");
	struct chat_msg response;
	static struct chat_msg_start msg_start = {
			.type = start_t,
			.client_id = -1,
			.text_length = 12 + 1,
			.text = "Hello server"
	};
	msg_start.client_qid = client_qid;

	if (chat_send(server_qid, &msg_start, CHAT_MSG_DATA_SIZE((&msg_start)) + msg_start.text_length) == -1)
	{
		perror("Couldn't send START msg to server");
		exit(EXIT_FAILURE);
	}
	printf("[OK]\n");


	printf("Waiting for assigned client_id... ");
	chat_receive(client_qid, &response, CHAT_MSG_DATA_SIZE_MAX, start_t);
	/* TODO!! Do some checking here */
	client_id = ((struct chat_msg_start *) &response)->client_id;
	printf("[OK] - registered as client %d\n", client_id);
}



/*
 * CLIENT CONSOLE ROUTINES
 */

int client_unicast_send_handle(void)
{
	static struct chat_msg msg;
	struct chat_msg_unicast *msg_unicast_p;
	char line[10];


	msg_unicast_p = ((struct chat_msg_unicast *) &msg);


	msg_unicast_p->type = unicast_t;
	msg_unicast_p->client_id = client_id;

	printf("Addressee client id: ");
	fgets(line, 10, stdin); 	/* Nah error checking */
	msg_unicast_p->addr_client_id = (int) strtoul(line, NULL, 10);

	printf("Message text: ");
	fgets(msg_unicast_p->text, CHAT_MSG_DATA_SIZE_MAX - CHAT_MSG_DATA_SIZE(msg_unicast_p), stdin);

	msg_unicast_p->text_length = strlen(msg_unicast_p->text) + 1;



	if (chat_send(server_qid, msg_unicast_p, CHAT_MSG_DATA_SIZE(msg_unicast_p) + msg_unicast_p->text_length) == -1)
	{
		perror("Couldn't send message");
		return -1;
	}

	return 0;
}



/*
 * CLIENT RECEIVER ROUTINES
 */
int client_unicast_receive_handle(struct chat_msg_unicast *msg, size_t msg_data_size)
{
	/*
	 * Sanity checks
	 */
	if (msg->type != unicast_t)
		return -1;

	if (msg->addr_client_id != client_id)
		return -1;

	/* ... */



	printf("Message from client %d: %s", msg->client_id, msg->text);

	return 0;
}


void on_client_stop(void)
{
	if (client_id == -1)
		return;

	static struct chat_msg_stop msg_stop = {
			.type = stop_t,
			.text_length = 14 + 1,
			.text = "Goodbye server"
	};
	msg_stop.client_id = client_id;

	if (chat_send(server_qid, &msg_stop, CHAT_MSG_DATA_SIZE((&msg_stop)) + msg_stop.text_length) == -1)
		perror("Couldn't send STOP message to server");

	if (chat_queue_destroy(client_qid) == -1)
		perror("Couldn't remove client's message queue");
}