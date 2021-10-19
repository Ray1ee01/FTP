#include "command_utils.h"
#include "socket_utils.h"
#include "client_utils.h"
#include "global.h"


CommandFilter Cmd_Filters[17]={
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
    {
        "RETR",
        CmdRETR
    },
    {
        "STOR",
        CmdSTOR
    },
    {
        "SYST",
        CmdSYST
    },
    {
        "TYPE",
        CmdTYPE
    },
    {
        "QUIT",
        CmdQUIT
    },
    {
        "LIST",
        CmdLIST
    },
    {
        "MKD",
        CmdMKD
    },
    {
        "CWD",
        CmdCWD
    },
    {
        "PWD",
        CmdPWD
    },
    {
        "RMD",
        CmdRMD
    },
    {
        "RNFR",
        CmdRNFR
    },
    {
        "RNTO",
        CmdRNTO
    }
};

void HandleCommand(const char* cmd,const char* params,Client* client)
{
    // printf("handle cmd %s\n",&cmd);
    for (int i=0;i<17;i++)
    {
        if (strcmp(cmd,Cmd_Filters[i].name)==0)
        {
            if (client->state!=TRANSFER)
            {
                printf("cmd %s\n",cmd);
                Cmd_Filters[i].fptr(params,client);
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
    if (strcmp(params,"anonymous")!=0)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        client->login=NEED_PASS;            // 不论是否已登录，强制更新登录状态。
        post_msg(client->conn_fd,331,NULL); // 即使已在线，也不做下线再登录的处理。
    }
    return;
}

void CmdPASS(const char* params,Client *client)
{
    // printf("get Cmd success\n");
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
        else if (h1<0||h1>255||h2<0||h2>255||h3<0||h3>255||h4<0||h4>255||p1<0||p1>255||p2<0||p2>255)
        {
            post_msg(client->conn_fd,501,NULL);
        }
        else if (client->state==TRANSFER)
        {
            //todo close tranfer connect
        }
        else
        {
            client->tran_mode=PORT;
            char ip[32];
            sprintf(ip,"%d.%d.%d.%d",h1,h2,h3,h4);
            //设置目标主机的ip和port
            memset(&(client->addr), 0, sizeof(client->addr));
            client->addr.sin_family = AF_INET;
            client->addr.sin_port = htons((p1<<8)+p2);
            // printf("%d\n",(p1<<8)+p2);
            if (inet_pton(AF_INET,ip, &(client->addr.sin_addr)) <= 0) {			//转换ip地址:点分十进制-->二进制
                printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
                post_msg(client->conn_fd,501,NULL);
            }
            else if (-1==(client->tran_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)))
            {
                printf("Error socket(): %s(%d)\n", strerror(errno), errno);
                post_msg(client->conn_fd, 500, NULL);
            }
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
        printf("to check port\n");
        while(1)
        {
            port = rand() % 45536+20000;
            if (CheckAvailPort(port)==0)break;
        }
        int h1,h2,h3,h4,p1,p2;
        sscanf(server_ip,"%d.%d.%d.%d" ,&h1,&h2,&h3,&h4);
        p1 = port / (1<<8);
        p2 = port % (1<<8);
        char msg[128];
        sprintf(msg,"(%d,%d,%d,%d,%d,%d)",h1,h2,h3,h4,p1,p2);
        
        printf("to listen tran port\n");
        client->tran_fd = ListenBind(port);
        // FD_SET(client->tran_fd,&read_set);
        // FD_SET(client->tran_fd,&write_set);
        post_msg(client->conn_fd,227,msg);
    }
    return;
}

void CmdRETR(const char *params,Client* client)
{   
    // todo if server is transfering another file to the client?
    // FILE *file;
    if (params==NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        FILE *file;
        // printf("%s\n",params);
        sprintf(client->filepath,"%s/%s",root,params);
        // printf("%s\n",client->filepath);
        if((file=fopen(client->filepath,"rb+"))==NULL)
        {
            post_msg(client->conn_fd,550,NULL);
        }
        else
        {
            fclose(file);
            client->state=ABOUT_TO_TRANSFER;
            //printf("retr true\n");
            //printf("%d\n",client->state);
            FD_SET(client->tran_fd,&write_set);
        }
    }
}

void CmdSTOR(const char *params,Client* client)
{   
    // todo if server is transfering another file to the client?
    // FILE *file;
    if (params==NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        FILE *file;
        // printf("%s\n",params);
        sprintf(client->filepath,"%s/%s",root,params);
        // printf("%s\n",client->filepath);
        if((file=fopen(client->filepath,"ab+"))==NULL)
        {
            post_msg(client->conn_fd,550,NULL);
        }
        else
        {
            fclose(file);
            printf("file open success\n");
            client->state=ABOUT_TO_TRANSFER;
            FD_SET(client->tran_fd,&read_set);
        }
    }
    return;
}
void CmdSYST(const char *params,Client* client)
{
    if (params!=NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    post_msg(client->conn_fd,215,NULL);
}
void CmdTYPE(const char *params,Client* client)
{
    if (strcmp(params,"I")!=0)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        post_msg(client->conn_fd,200,NULL);
    }
}
void CmdQUIT(const char *params,Client* client)
{
    if (params!=NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        post_msg(client->conn_fd,221);
        FD_CLR(client->conn_fd,&read_set);
        FD_CLR(client->tran_fd,&read_set);
        FD_CLR(client->conn_fd,&write_set);
        FD_CLR(client->tran_fd,&write_set);
        Init_Client(client);
    }
    return;
}
void CmdLIST(const char *params,Client* client)
{
    // char path[256];
    if(params==NULL)
    {
        if(strlen(client->curdir)==0)
        {
            sprintf(client->filepath,"%s",root);
        }
        else
        {
            sprintf(client->filepath,"%s",client->curdir);
        }
    }
    else
    {
        if(strlen(client->curdir)==0)
        {
            sprintf(client->filepath,"%s/%s",root,params);
        }
        else
        {
            sprintf(client->filepath,"%s/%s",client->curdir,params);
        }
    }
    client->list=LIST;
    client->state=ABOUT_TO_TRANSFER;
    FD_SET(client->tran_fd,&write_set);
    return;
}

void CmdMKD(const char *params,Client* client)
{
    char path[256];
    if (params == NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    sprintf(path,"%s/%s",root,params);
    if (mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) 
    {
        post_msg(client->conn_fd, 250, path);
    }
    else 
    {
        post_msg(client->conn_fd, 550, NULL);
    }
    return ;
}

void CmdCWD(const char *params,Client* client)
{
    char path[256];
    if (params == NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        sprintf(path,"%s/%s",root,params);
        printf("%s",client->curdir);
        if(access(path,0)!=0)
        {
            post_msg(client->conn_fd,550,NULL);
        }
        else
        {
            strcpy(client->curdir,path);
            post_msg(client->conn_fd,250,NULL);
        }
    }
    return;
}

void CmdPWD(const char *params,Client* client)
{
    if (params!=NULL)
    {
        printf("handle PWD nullptr\n");
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        printf("handle PWD\n");
        post_msg(client->conn_fd,257,client->curdir);
    }
}

void CmdRMD(const char *params,Client* client)
{
    char path[256];
    if (params==NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        if(strlen(client->curdir)==0)
        {
            sprintf(path,"%s/%s",root,params);
        }
        else
        {
            sprintf(path,"%s/%s",client->curdir,params);
        }
        printf("%s\n",path);
        if(access(path,0)==-1)
        {
            post_msg(client->conn_fd,550,NULL);
        }
        else
        {
            if(rmdir(path)!=0)
            {
                post_msg(client->conn_fd,550,NULL);
            }
            else
            {
                post_msg(client->conn_fd,250,NULL);
            }
        }
    }
    return;
}
void CmdRNFR(const char *params,Client* client)
{
    char path[256];
    if (params==NULL)
    {
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        if(strlen(client->curdir)==0)
        {
            sprintf(path,"%s/%s",root,params);
        }
        else
        {
            sprintf(path,"%s/%s",client->curdir,params);
        }
        printf("%s\n",path);
        if(access(path,0)!=0)
        {
            post_msg(client->conn_fd,550,NULL);
        }
        else
        {
            sprintf(client->rnfile,"%s",path);
            client->state=RNFR;
            post_msg(client->conn_fd,350,NULL);
        }
    }
    return;
}
void CmdRNTO(const char *params,Client* client)
{
    if (client->state!=RNFR)
    {
        printf("state error\n");
        post_msg(client->conn_fd,503,NULL);
    }
    else if(params ==NULL)
    {
        printf("params error\n");
        post_msg(client->conn_fd,504,NULL);
    }
    else
    {
        char cmd[256];
        sprintf(cmd,"mv %s %s",client->rnfile,params);
        printf("%s\n",cmd);
        if (system(cmd)==-1)
        {
            post_msg(client->conn_fd,550,NULL);
        }
        else
        {
            post_msg(client->conn_fd,250,NULL);
        }
    }
}

