/**
 * Client side is written quite lazily so you will need to fill in some gaps and do a little cleanup.
 *
 * Nature of client application necessitates use of multiple processes/threads.
 *   Even though it is a little ahead of your current sysopy progress i chose POSIX's pthread instead of fork to achieve concurrency.
 *   I just think that this example calls for pthread usage instead of clunky fork, but you can use whatever you see fit.
 *
 *   Have fun :)
 */
#define _GNU_SOURCE    /* obtain strchrnul() declaration */

#include "system_V/chat.h"
#include "system_V/client_routines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>
#include <errno.h>


int server_qid;
int client_qid;
int client_id;


static void client_run(void);

static void *client_console_thread_start(void *args);

static void *client_receiver_thread_start(void *args);



int main(int argc, char **argv)
{
	client_setup();

	client_run();

	exit(EXIT_SUCCESS);
}


void client_run(void)
{
	int error;
	pthread_attr_t attr;
	pthread_t client_console_thread_id;
	pthread_t client_receiver_thread_id;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


	error = pthread_create(&client_console_thread_id, &attr, client_console_thread_start, NULL);
	if (error != 0)
	{
		errno = error;
		perror("Failed to create console thread");
		exit(EXIT_FAILURE);
	}


	error = pthread_create(&client_receiver_thread_id, &attr, client_receiver_thread_start, NULL);
	if (error != 0)
	{
		errno = error;
		perror("Failed to create receiver thread");
		exit(EXIT_FAILURE);
	}


	pthread_exit(NULL);
}


static void *client_console_thread_start(void *args)
{
	char *line_p;
	size_t line_size;

	line_size = 80;
	line_p = malloc(sizeof(*line_p) * line_size);


	putchar('>');
	while ( getline(&line_p, &line_size, stdin) != -1 )
	{
		/*
		 * If you want more portable code replace strchrnul() with strchr() and provide some error handling  */
		*strchrnul(line_p, '\n') = '\0';
		*strchrnul(line_p, ' ') = '\0';

		if (strcmp("unicast", line_p) == 0)
			client_unicast_send_handle();
		else if (strcmp("stop", line_p) == 0)
			exit(EXIT_SUCCESS);


		putchar('>');
	}


	return NULL;
}


static void *client_receiver_thread_start(void *args)
{
	struct chat_msg message;
	size_t message_data_size;

	message_data_size = chat_receive(client_qid, &message, CHAT_MSG_DATA_SIZE_MAX, -999999);

	switch (message.type)
	{
		case stop_t:
			exit(EXIT_SUCCESS);

		case unicast_t:
			client_unicast_receive_handle((struct chat_msg_unicast *) &message, message_data_size);
			break;

		case start_t:
		case foo_t:
		default:
			break;
	}



	return NULL;
}


