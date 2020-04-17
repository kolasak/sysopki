#ifndef CLIENT_SYSTEM_V_ROUTINES_H
#define CLIENT_SYSTEM_V_ROUTINES_H


void client_setup(void);

void on_client_stop(void);

int client_unicast_send_handle(void);

int client_unicast_receive_handle(struct chat_msg_unicast *msg, size_t msg_data_size);


#endif /* CLIENT_SYSTEM_V_ROUTINES_H */
