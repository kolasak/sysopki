/*
 * You can see active POSIX queues in /dev/mqueue/
 */


#include "POSIX/chat.h"

#include <mqueue.h>


/* There is still some room for improvement of function generalization.
 *   Eg. chat_server_queue_init() & chat_client_queue_init() (and so on) do almost the same thing.
 *   As an exercise try to group these functions.  */


int chat_server_queue_init()
{
	static const struct mq_attr attr = {
			.mq_maxmsg = CHAT_MSG_CNT_MAX,
			.mq_msgsize = CHAT_MSG_LEN_MAX,
	};

	return mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
}


int chat_server_queue_open()
{
	static const struct mq_attr attr = {
			.mq_maxmsg = CHAT_MSG_CNT_MAX,
			.mq_msgsize = CHAT_MSG_LEN_MAX,
	};

	return mq_open(SERVER_QUEUE_NAME, O_WRONLY, 0644, &attr);
}


int chat_client_queue_init(const char *name)
{
	static const struct mq_attr attr = {
			.mq_maxmsg = CHAT_MSG_CNT_MAX,
			.mq_msgsize = CHAT_MSG_LEN_MAX,
	};


	return mq_open(name, O_CREAT | O_RDONLY, 0644, &attr);
}


int chat_client_queue_open(const char *name)
{
	static const struct mq_attr attr = {
			.mq_maxmsg = CHAT_MSG_CNT_MAX,
			.mq_msgsize = CHAT_MSG_LEN_MAX,
	};

	return mq_open(name, O_WRONLY, 0644, &attr);

}


int chat_queue_destroy(const char *name)
{
	return mq_unlink(name);
}


int chat_queue_close(mqd_t mqdes)
{
	return mq_close(mqdes);

}


int chat_send(mqd_t mqdes, void *msg, size_t msg_size, unsigned int priority)
{
	return mq_send(mqdes, msg, msg_size, priority);
}


int chat_receive(mqd_t mqdes, void *msg, size_t msg_size, unsigned int *priority)
{
	return mq_receive(mqdes, msg, msg_size, priority);
}
