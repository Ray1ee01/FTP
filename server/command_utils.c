#include "command_utils.h"
#include "global.h"


CommandFilter Cmd_Filters[5]={
    {
        "ABOR",
        CmdABOR
    },
    {
        "USER",
        CmdUSER
    },
    {
        "PASS",
        CmdPASS
    },
    {
        "PORT",
        CmdPORT
    },
    {
        "PASV",
        CmdPASV
    },
};

void HandleCommand(const char* cmd,const char* params,Client* client)
{
    for (int i=0;i<3;i++)
    {
        if (strcmp(cmd,Cmd_Filters[i].name)==0)
        {
            if (client->state==-1)
            {
                Cmd_Filters[i].fptr(params,client);
                client->state=-1;
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
    if(client->login!=NEED_PASS)
    {
        post_msg(client->conn_fd,503,NULL);
    }
    else
    {
        client->login=LOGIN;
        post_msg(client->conn_fd,230,NULL);
    }
    return;
}

void CmdPORT(const char* params,Client *client)
{
    if (params==NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        int h1,h2,h3,h4,p1,p2;

        if (6!=sscanf(params,"%d,%d,%d,%d,%d,%d",&h1,&h2,&h3,&h4,&p1,&p2))
        {
            post_msg(client->conn_fd,501,NULL);
        }
        //The RFC 959 formal syntax does not allow any of these numbers to be 0. The formal syntax is wrong.
        else if (h1<=0||h1>255||h2<=0||h2>255||h3<=0||h3>255||h4<=0||h4>255||p1<=0||p1>255||p2<=0||p2>255)
        {
            post_msg(client->conn_fd,501,NULL);
        }
        else if (client->state==TRANSFER)
        {
            //todo close tranfer connect
        }
        else
        {
            char ip[32];
            sprintf(ip,"%d.%d.%d.%d",h1,h2,h3,h4);
            //设置目标主机的ip和port
            memset(&(client->addr), 0, sizeof(client->addr));
            client->addr.sin_family = AF_INET;
            client->addr.sin_port = htons((p1<<8)+p2);
            if (inet_pton(AF_INET,ip, &(client->addr.sin_addr)) <= 0) {			//转换ip地址:点分十进制-->二进制
                printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
                post_msg(client->conn_fd,501,NULL);
            }
            else if (-1==(client->tran_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)))
            {
                printf("Error socket(): %s(%d)\n", strerror(errno), errno);
                post_msg(client->conn_fd, 500, NULL);
            }
            //to do manage tran_fd

            post_msg(client->conn_fd,200,"PORT success.");
        }
    }
    return;
}

void CmdPASV(const char *params,Client* client)
{
    if (params!=NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        client->tran_mode=PASV;
        if (client->state==TRANSFER)
        {
            //todo close tranfer connect
        }
        int port;
        while(1)
        {
            port = rand() % 45536+20000;
            if (CheckAvailPort(port)==0)break;
        }
        int h1,h2,h3,h4,p1,p2;
        sscanf(server_ip,"%d.%d.%d.%d" ,&h1,&h2,&h3,&h4);
        p1 = port / (1<<8);
        p2 = port % (1<<8);
        char* msg[128];
        sprintf(msg,"(%d,%d,%d,%d,%d,%d)",h1,h2,h3,h4,p1,p2);
        int tran_fd;
        tran_fd = ListenBind(port);
        // to do manage fd
        post_msg(client->conn_fd,227,msg);
    }
    return;
}