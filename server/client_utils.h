/*
client utils
*/
#ifndef CLIENT_UTILS
#define CLIENT_UTILS

#include "global.h"

void Init_Client(Client* client);

void Setup_Client(Client* client,int conn_fd);

#endif