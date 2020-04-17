#ifndef SERVER_SYSTEM_V_ROUTINES_H
#define SERVER_SYSTEM_V_ROUTINES_H


#define HANDLE_FAIL_WITH_MESSAGE(msg)    do {  \
                                            printf("\n  %s: %s\n", __func__, msg);  \
                                            return -1;  \
                                        } while(0);


void server_setup(void);

int server_client_stop_handle(struct chat_msg_stop *msg, size_t msg_data_size);

int server_client_start_handle(struct chat_msg_start *msg, size_t msg_data_size);

int server_client_unicast_handle(struct chat_msg_unicast *msg, size_t msg_data_size);

void on_server_stop(void);


#endif /* SERVER_SYSTEM_V_ROUTINES_H */
