#include "command_utils.h"
#include "global.h"
struct CommandFilter[3];

CommandFilter[0]={
    "ABOR",
    CmdABOR
};
CommandFilter[1]={
    "USER",
    CmdUSER
};
CommandFilter[2]={
    "PASS",
    CmdPASS
};
void HandleCommand(const char* cmd,const char* params,Client *client)
{
    for (int i=0;i<3;i++)
    {
        if (strcmp(cmd,CommandFilter[i].name)==0)
        {
            if (client->state==-1)
            {
                CommandFilter[i].fptr(params,client);
                client->state=-1
                break;
            }
            else
            {
                // 如果正在处理指令，该怎么办？
                break;
            }
        }
    }
}
void CmdABOR(const char* params,Client *client)
{
    if (client->state!=-1)
    {
        // todo 终止当前指令
        post_msg(client->conn_fd,426,NULL);
    }
    else
    {
        post_msg(client->conn_fd,226,NULL);
    }
    return;
}


void CmdUSER(const char* params,Client *client)
{
    if (params!="anonymous")
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        client->login=LOGIN;                // 不论是否已登录，强制更新登录状态。
        post_msg(client->conn_fd,331,NULL); // 即使已在线，也不做下线再登录的处理。
    }
    return;
}

void CmdPASS(const char* params,Client *client)
{
    if(client->log_in!=NEED_PASS)
    {
        post_msg(client->conn_fd,503,NULL);
    }
    else
    {
        client->log_in=LOGIN;
        post_msg(client->conn_fd,230,NULL)
    }
    return;
}

