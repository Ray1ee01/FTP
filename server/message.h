#include "global.h"

void get_msg(int fd, char *msg, int max_len);

void post_msg(int fd, int code, char *msg);

int send_file(Client *client, FILE *file, char* buf);