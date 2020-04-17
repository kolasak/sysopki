#ifndef CHAT_SYSTEM_V_H
#define CHAT_SYSTEM_V_H


#include <sys/ipc.h>
#include <glob.h>


#define CLIENT_CNT_MAX 10

#define CHAT_MSG_DATA_SIZE_MAX 256

#define CHAT_MSG_DATA_SIZE(msg) (sizeof(*msg) - sizeof(msg->type))



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



/**
 * Common struct for receiving any message type,
 *   later use appropriate struct for handling received data.
 **/
struct chat_msg
{
	long type;

	char data[CHAT_MSG_DATA_SIZE_MAX];
};


/**
 * Different structs for different message types
 **/
struct chat_msg_start
{
	long type;

	int client_id;
	int client_qid;

	size_t text_length;
	char text[];
};

struct chat_msg_stop
{
	long type;

	int client_id;

	size_t text_length;
	char text[];
};

struct chat_msg_unicast
{
	long type;

	int client_id;
	int addr_client_id;

	size_t text_length;
	char text[];
};

/* ... */





int chat_server_queue_init();

int chat_server_queue_open();

int chat_client_queue_init();

int chat_queue_destroy(int qid);

int chat_send(int qid, void *msg, size_t msg_data_size);

int chat_receive(int qid, void *msg, size_t msg_data_size_max, long type);


#endif /* CHAT_SYSTEM_V_H */
