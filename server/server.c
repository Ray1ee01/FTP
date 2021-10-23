



#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <getopt.h>

#include "socket_utils.h"
#include "message.h"
#include "global.h"
#include "command_utils.h"
#include "client_utils.h"



int main(int argc, char **argv) {
    int listen_fd, conn_fd;		//监听socket和连接socket不一样，后者用于数据传输
    // struct sockaddr_in addr;
    // int p;
    // int len;
    int port =6789;
    char sentence[256];
    // char buf[BUFFER_SIZE];
    // strcpy(root,"/tmp");
    strcpy(root,"/home/ubuntu/test");
    GetLocalIP(server_ip);
    // printf("%s",server_ip);
    // printf("Start receive argu\n");    
    // 接收命令行参数
    // honor code: https://blog.csdn.net/pengrui18/article/details/8078813
    const char *optstring="p:r:";
    int opt;
    const struct option opts[] = 
    {
        {"port", required_argument, NULL,'p'},
        {"root",required_argument, NULL,'r'},
        {0,0,0,0},
    };
    while((opt = getopt_long_only(argc,argv,optstring,opts,NULL))!=-1)
    {
        switch(opt)
        {
        case 'p':
            // printf("Port: %s.\n",optarg);
            // TODO
            // 检测port是否可用 or 合法
            port=atoi(optarg);
            break;
        case 'r':
            // printf("Directory: %s.\n",optarg);
            if (access(optarg, 0) == -1)
            {
                printf("Directory %s does not exist or Permission denied \n",optarg);
                return 0;
            }
            strcpy(root,optarg);
            break;
        case '?':
            printf("Bad argument.\n");
            return 0;
        }
    }
    listen_fd = ListenBind(port); // listen parameter?

    for(int i=0;i<MAX_CLIENTS;i++)
    {
        Init_Client(&client_entities[i]);
    }

    // init FD_SET
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_SET(listen_fd,&read_set);
    
    int result;
    // https://blog.csdn.net/y396397735/article/details/55004775
    fd_set t_read;
    fd_set t_write;

    while (1) {
        t_read=read_set;
        t_write=write_set;
        // 无限期阻塞，并测试文件描述符变动
        result = select(FD_SETSIZE,&t_read,&t_write,(fd_set*) 0,NULL);
        // printf("result %d\n",result);
        if (result<1)
        {
            printf("Error Select\n");
            return 0;
        }
        // 建立连接
        if (FD_ISSET(listen_fd,&t_read))
        {
            printf("listen_fd %d\n",listen_fd);
            conn_fd=AcceptConnection(listen_fd);
            if (conn_fd==-1)
            {
                printf("Error accept\n");
            }
            else
            {
                for(int i=0;i<MAX_CLIENTS;i++)
                {
                    if(client_entities[i].conn_fd==-1)
                    {
                        client_entities[i].conn_fd=conn_fd;
                        FD_SET(client_entities[i].conn_fd,&write_set);
                        FD_SET(client_entities[i].conn_fd,&read_set);
                        post_msg(client_entities[i].conn_fd,220,NULL);
                        break;
                    }
                    else
                    {
                        printf("%d\n",client_entities[i].conn_fd);
                    }
                    if(i==MAX_CLIENTS-1)
                    {
                        printf("No spare clients connection to use");
                        close(conn_fd);
                    }
                }
            }
            // if(result==1)continue;
        }
        for(int i=0;i<MAX_CLIENTS;i++)
        {
            if(client_entities[i].conn_fd==-1)
            {
                continue;
            }
            // printf("1conn_fd: %d\n",client_entities[i].conn_fd);
            if (FD_ISSET(client_entities[i].conn_fd,&t_read))
            {
                printf("2conn_fd: %d\n",client_entities[i].conn_fd);
                memset(sentence,0,sizeof sentence);
                get_msg(client_entities[i].conn_fd,sentence,255);
                if(strlen(sentence)<=0)
                {
                    printf("Error read(): %s(%d)\n", strerror(errno), errno);
                    close(client_entities[i].conn_fd);
                    client_entities[i].conn_fd=-1;
                    continue;
                }
                else
                {
                    for(int j=strlen(sentence)-1;j>=1;j--)
                    {
                        if(sentence[j]=='\n' && sentence[j-1]=='\r')
                        {
                            sentence[j-1]='\0';
                            break;
                        }
                    }
                    char cmd[16];
                    char params[64];
                    memset(cmd,0,sizeof cmd);
                    memset(params,0,sizeof params);
                    int cmd_param = sscanf(sentence, "%s %s", cmd,params);
                    // printf("%s,%s\n",cmd,params);
                    if(cmd_param<=0)
                    {
                        post_msg(client_entities[i].conn_fd,500,NULL);
                    }
                    else if (cmd_param==1)
                    {
                        HandleCommand(cmd,NULL,&client_entities[i]);
                    }
                    else
                    {
                        HandleCommand(cmd,params,&client_entities[i]);
                    }
                }
            }
        }
    }
    close(listen_fd);
}

