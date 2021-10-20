#include "global.h"

#ifndef MESSAGE
#define MESSAGE
void get_msg(int fd, char *msg, int max_len);
void post_msg(int fd, int code, char *msg);

int get_data(int fd, char *msg, int max_len);
int post_data(int fd, char *msg, int max_len);


void* send_file(void *args);
void* recv_file(void *args);
void* send_list(void *args);

#endif