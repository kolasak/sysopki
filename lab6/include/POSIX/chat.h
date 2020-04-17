#ifndef CHAT_POSIX_H
#define CHAT_POSIX_H

#include <mqueue.h>



#define CLIENT_CNT_MAX 10

#define CHAT_MSG_CNT_MAX 10
#define CHAT_MSG_LEN_MAX 256


#define SERVER_QUEUE_NAME "/server_POSIX"



#define CHAT_MSG_PRIO_1 0b00001000u
#define CHAT_MSG_PRIO_2 0b00010000u
#define CHAT_MSG_PRIO_3 0b00011000u

enum chat_msg_type
{
	stop_t = CHAT_MSG_PRIO_1 | 0b000u,
	start_t = CHAT_MSG_PRIO_1 | 0b001u,
	unicast_t = CHAT_MSG_PRIO_2 | 0b000u,
	foo_t = CHAT_MSG_PRIO_2 | 0b001u,
	/* ... */
};



/*
 * All messages must have enough space to accommodate message of mq_msgsize size, specified in attribute of queue(mq_attr and mq_open)
 */
struct chat_msg
{
	char data[CHAT_MSG_LEN_MAX];
};

struct chat_msg_start
{
	int client_id;
	char name[CHAT_MSG_LEN_MAX - sizeof(int)];
};

struct chat_msg_stop
{
	int client_id;
	char text[CHAT_MSG_LEN_MAX - sizeof(int)];
};

struct chat_msg_unicast
{
	int client_id;
	int addr_client_id;
	char text[CHAT_MSG_LEN_MAX - 2 * sizeof(int)];
};



int chat_server_queue_init();

int chat_server_queue_open();

int chat_client_queue_init(const char *name);

int chat_client_queue_open(const char *name);

int chat_queue_destroy(const char *name);

int chat_queue_close(mqd_t mqdes);

int chat_send(mqd_t mqdes, void *msg, size_t msg_size, unsigned int priority);

int chat_receive(mqd_t mqdes, void *msg, size_t msg_size, unsigned int *priority);


#endif /* CHAT_POSIX_H */
