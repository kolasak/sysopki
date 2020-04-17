/*
 * To show active system V message queues use:
 *   ``` $ ipcs ```
 */


#include "system_V/chat.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


/* I just prefer it to ftok() approach */
const key_t server_key = 0xbadcafe;


int chat_server_queue_init()
{
	return msgget(server_key, IPC_CREAT | 0666); /* IPC_CREAT | IPC_EXCL | */
}


int chat_server_queue_open()
{
	return msgget(server_key, 0666);
}


int chat_client_queue_init()
{
	return msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666);
}


int chat_queue_destroy(int qid)
{
	return msgctl(qid, IPC_RMID, NULL);
}


int chat_send(int qid, void *msg, size_t msg_data_size)
{
	return msgsnd(qid, msg, msg_data_size, 0);
}


int chat_receive(int qid, void *msg, size_t msg_data_size_max, long type)
{
	size_t data_size;
	data_size = msgrcv(qid, msg, msg_data_size_max, type, 0);

	if (data_size == -1)
		return -1;

	if (data_size > CHAT_MSG_DATA_SIZE_MAX)
		return -1;

	/*
	 * Null terminate received message!
	 *
	 * Received message doesn't need to be used as string,
	 *   but zero terminate it, just in case.
	 * Edge case where data structure size == CHAT_MSG_DATA_SIZE_MAX is something
	 *   to consider when building said structures. */
	((struct chat_msg *) msg)->data[data_size < CHAT_MSG_DATA_SIZE_MAX ? data_size : CHAT_MSG_DATA_SIZE_MAX - 1] = '\0';


	return data_size;
}
