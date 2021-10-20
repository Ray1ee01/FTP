#include "global.h"
#include "client_utils.h"


void Init_Client(Client* client)
{
    client->conn_fd=-1;
    client->tran_fd=-1;
    client->login=NOT_LOGIN;
    client->tran_mode=NOT_SET;
    client->state=NOT_SET;
    memset(client->filepath,0,sizeof(client->curdir));
    memset(client->curdir,0,sizeof(client->curdir));
    strcpy(client->curdir,root);
    memset(client->rnfile,0,sizeof(client->curdir));
    client->offset=0;
    client->list=NOT_LIST;
    // how to set socket addr?
    return;
}

void Setup_Client(Client* client,int conn_fd)
{
    client->conn_fd=conn_fd;
    return;
}