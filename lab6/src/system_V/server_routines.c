#include "system_V/chat.h"
#include "system_V/server_routines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>



extern int server_qid;
extern int clients_qid[CLIENT_CNT_MAX];



static void signal_handler(int signum)
{
	exit(EXIT_SUCCESS);
}

void server_setup(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);

	atexit(on_server_stop);
	signal(SIGINT, signal_handler);


	printf("Obtaining server's message queue id... ");
	server_qid = chat_server_queue_init();
	if (server_qid == -1)
	{
		perror("Couldn't create server's message queue");
		exit(EXIT_FAILURE);
	}
	printf("[OK] - qid: %d", server_qid);
}


/*
 * SERVER ROUTINE HANDLERS
 */

int server_client_start_handle(struct chat_msg_start *msg, size_t msg_data_size)
{
	/*
	 * Sanity checks
	 */
	if (msg->type != start_t)
		HANDLE_FAIL_WITH_MESSAGE("Wrong type")

	if (msg_data_size < CHAT_MSG_DATA_SIZE(msg))
		HANDLE_FAIL_WITH_MESSAGE("Incomplete message")

	if (msg->client_qid < 0)
		HANDLE_FAIL_WITH_MESSAGE("Incorrect client id")

	if (msg->text_length < 0)
		HANDLE_FAIL_WITH_MESSAGE("Incorrect client id")

	if (msg_data_size > CHAT_MSG_DATA_SIZE(msg) + msg->text_length)
		HANDLE_FAIL_WITH_MESSAGE("Received to many bytes")


	/*
	 * Handler routine
	 */
	for (int client_id = 0; client_id < CLIENT_CNT_MAX; client_id++)
	{
		if (clients_qid[client_id] != -1)
			continue;

		/* Lazily reuse available msg_start structure */
		msg->client_id = client_id;
		if (chat_send(msg->client_qid, msg, msg_data_size) == -1)
		{
			printf("\nCouldn't send START ACK to client(qid: %d)", msg->client_qid);
			perror("");
			return -1;
		}

		clients_qid[client_id] = msg->client_qid;
		printf("\nClient %d (qid: %d) start message: %s\n", client_id, msg->client_qid, msg->text);
		return 0;
	}


	HANDLE_FAIL_WITH_MESSAGE("No slots for new clients")
}


int server_client_stop_handle(struct chat_msg_stop *msg, size_t msg_data_size)
{
	/*
	 * Sanity checks
	 */
	if (msg->type != stop_t)
		HANDLE_FAIL_WITH_MESSAGE("Wrong type")

	if (msg_data_size < CHAT_MSG_DATA_SIZE(msg))
		HANDLE_FAIL_WITH_MESSAGE("Incomplete message")

	if (msg->client_id < 0)
		HANDLE_FAIL_WITH_MESSAGE("Incorrect client id")

	if (msg->client_id >= CLIENT_CNT_MAX)
		HANDLE_FAIL_WITH_MESSAGE("Incorrect client id")

	if (msg->text_length < 0)
		HANDLE_FAIL_WITH_MESSAGE("Incorrect text_length")

	if (msg_data_size > CHAT_MSG_DATA_SIZE(msg) + msg->text_length)
		HANDLE_FAIL_WITH_MESSAGE("Received to many bytes")

	if (clients_qid[msg->client_id] == -1)
		HANDLE_FAIL_WITH_MESSAGE("Client not registered")


	/*
	 * Handler routine
	 */
	printf("\nClient %d (qid: %d) stop message: %s\n", msg->client_id, clients_qid[msg->client_id],
		   msg->text);    /* 'msg_stop->text' is expected to be '\0' terminated since chat_receive() takes care of that, but maybe it should be done here. */

	clients_qid[msg->client_id] = -1;

	return 0;
}


int server_client_unicast_handle(struct chat_msg_unicast *msg, size_t msg_data_size)
{
	/*
	 * Sanity checks
	 */
	if (msg->type != unicast_t)
		HANDLE_FAIL_WITH_MESSAGE("Wrong type")

	if (msg_data_size < CHAT_MSG_DATA_SIZE(msg))
		HANDLE_FAIL_WITH_MESSAGE("Incomplete message")

	if (msg->client_id < 0 || msg->client_id >= CLIENT_CNT_MAX)
		HANDLE_FAIL_WITH_MESSAGE("Incorrect client id")

	if (msg->addr_client_id < 0 || msg->addr_client_id >= CLIENT_CNT_MAX)
		HANDLE_FAIL_WITH_MESSAGE("Incorrect addressee client id")

	if (msg->text_length < 0)
		HANDLE_FAIL_WITH_MESSAGE("Incorrect text_length")

	if (msg_data_size > CHAT_MSG_DATA_SIZE(msg) + msg->text_length)
		HANDLE_FAIL_WITH_MESSAGE("Received to many bytes")

	if (clients_qid[msg->client_id] == -1)
		HANDLE_FAIL_WITH_MESSAGE("Client not registered")

	if (clients_qid[msg->addr_client_id] == -1)
		HANDLE_FAIL_WITH_MESSAGE("Addressee client not registered")


	/*
	 * Handler routine
	 */
	if (chat_send(clients_qid[msg->addr_client_id], msg, msg_data_size) == -1)
	{
		printf("\nFailed to send message from client %d(qid: %d) to client %d(qid: %d)\n", msg->client_id,
			   clients_qid[msg->client_id], msg->addr_client_id, clients_qid[msg->addr_client_id]);
		perror("");
		return -1;
	}

	return 0;
}



/*
 * SERVER STOP SUBROUTINES
 */

static void server_send_stop_request(int client_id)
{
	static struct chat_msg_stop msg_stop = {
			.type = stop_t,
			.text_length = 17 + 1,
			.text = "Server is closing",
	};
	msg_stop.client_id = client_id;

	if (chat_send(clients_qid[client_id], &msg_stop, sizeof(msg_stop) + msg_stop.text_length) == -1)
		perror("\nCouldn't send stop request");
}


void on_server_stop(void)
{
	int stop_requests_sent_cnt = 0;

	/*
	 * Send stop requests to active clients.
	 **/
	printf("\nSending stop request to clients... ");
	for (int cli_id = 0; cli_id < CLIENT_CNT_MAX; cli_id++)
	{
		if (clients_qid[cli_id] == -1)
			continue;

		printf("%d(qid: %d)  ", cli_id, clients_qid[cli_id]);
		server_send_stop_request(cli_id);

		stop_requests_sent_cnt++;
	}
	printf("[OK]\n");


	/*
	 * Wait for acks from clients.
	 **/
	printf("Receiving stop acknowledgements from clients...\n");
	static struct chat_msg response;

	while ( --stop_requests_sent_cnt >= 0 )
	{
		size_t data_len;

		data_len = chat_receive(server_qid, &response, CHAT_MSG_DATA_SIZE_MAX,
								stop_t);    /* Grab only specified type of messages */
		if (data_len == -1)
		{
			perror("chat_receive - stop ack");
			return; /* Don't use exit() here :) */
		}

		server_client_stop_handle((struct chat_msg_stop *) &response, data_len);
	}
	printf("[OK]\n");


	/*
	 * Remove server side queue.
	 **/
	printf("Removing server's message queue... ");
	if (chat_queue_destroy(server_qid) == -1)
	{
		perror("Couldn't remove server's message queue");
		return;
	}
	printf("[OK]\n");
}