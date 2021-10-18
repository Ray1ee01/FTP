#include "global.h"

void get_msg(int fd, char *msg, int max_len);
void post_msg(int fd, int code, char *msg);

int get_data(int fd, char *msg, int max_len);
int post_data(int fd, char *msg, int max_len);


int send_file(Client *client, FILE *file, char* buf);
int recv_file(Client *client, FILE *file, char* buf);