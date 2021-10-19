



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
    char buf[BUFFER_SIZE];
    strcpy(root,"/tmp");
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
    // printf("%d\n",listen_fd);
    // init all clients
    if(listen_fd==-1)
    {
        printf("Listen error");
        return 0;
    }
    for(int i=0;i<MAX_CLIENTS;i++)
    {
        Init_Client(&client_entities[i]);
    }

    // init FD_SET
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_SET(listen_fd,&read_set);
    // if(FD_ISSET(listen_fd,&read_set))printf("listen fd is set\n1");
    // 持续监听连接请求
    int result;
    // https://blog.csdn.net/y396397735/article/details/55004775
    fd_set t_read;
    fd_set t_write;

    while (1) {
        t_read=read_set;
        t_write=write_set;
        // 无限期阻塞，并测试文件描述符变动
        result = select(FD_SETSIZE,&t_read,&t_write,(fd_set*) 0,NULL);
        if (result<1)
        {
            printf("Error Select");
            return 0;
        }
        // 建立连接
        if (FD_ISSET(listen_fd,&t_read))
        {
            conn_fd=AcceptConnection(listen_fd);
            if (conn_fd==-1)
            {
                printf("Error accept");
            }
            else
            {
                // printf("Conn_fd build\n");
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
                    if(i==MAX_CLIENTS-1)
                    {
                        printf("No spare clients connection to use");
                        close(conn_fd);
                    }
                }
            }
        }
        for(int i=0;i<MAX_CLIENTS;i++)
        {
            if(client_entities[i].conn_fd==-1)
            {
                continue;
            }
            if (FD_ISSET(client_entities[i].conn_fd,&t_read))
            {
                // printf("get avail client %d\n",i);
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
            if(client_entities[i].tran_fd==-1)
            {
                continue;
            }
            if(FD_ISSET(client_entities[i].tran_fd,&t_read))
            {
                if(client_entities[i].tran_mode== PASV)
                {
                    printf("%d\n",client_entities[i].state);
                    if (client_entities[i].state==ABOUT_TO_TRANSFER)
                    {
                        post_msg(client_entities[i].conn_fd,150,NULL);
                        int tran_fd=AcceptConnection(client_entities[i].tran_fd);
                        printf("accept success\n");
                        if (tran_fd==-1)
                        {
                            printf("tran_fd error\n");
                            post_msg(client_entities[i].conn_fd,425,NULL);
                            continue;
                        }
                        FD_SET(tran_fd,&read_set);
                        client_entities[i].tran_fd=tran_fd;
                        client_entities[i].state=TRANSFER;
                    }
                    else if(client_entities[i].state==TRANSFER)
                    {
                        printf("start transfer\n");
                        FILE* file=fopen(client_entities[i].filepath,"ab+");
                        fseek(file,client_entities[i].offset,SEEK_SET);
                        recv_file(&client_entities[i],file,buf);
                    }
                }
                else if(client_entities[i].tran_mode==PORT)
                {
                    if(client_entities[i].state==ABOUT_TO_TRANSFER)
                    {
                        if(connect(client_entities[i].tran_fd, (struct sockaddr *)&client_entities[i].addr, sizeof(client_entities[i].addr))==-1)
                        {
                            close(client_entities[i].tran_fd);
                            printf("port connect error\n");
                            post_msg(client_entities[i].conn_fd,425,NULL);
                        }
                        else
                        {
                            post_msg(client_entities[i].conn_fd,150,NULL);
                            client_entities[i].state=TRANSFER;
                            FILE* file=fopen(client_entities[i].filepath,"ab+");
                            fseek(file,client_entities[i].offset,SEEK_SET);
                            recv_file(&client_entities[i],file,buf);
                        }
                    }
                    else if(client_entities[i].state==TRANSFER)
                    {
                        // printf("PORT RECV\n");
                        FILE* file=fopen(client_entities[i].filepath,"ab+");
                        fseek(file,client_entities[i].offset,SEEK_SET);
                        recv_file(&client_entities[i],file,buf);
                    }
                }
                // else
                // {
                //     FD_CLR(client_entities[i].tran_fd,&read_set);
                //     FD_CLR(client_entities[i].tran_fd,&write_set);
                //     client_entities[i].tran_fd=-1;
                //     client_entities[i].tran_mode=NOT_SET;
                // }
            }
            else if(FD_ISSET(client_entities[i].tran_fd,&t_write))
            {
                if(client_entities[i].tran_mode== PASV)
                {
                    if (client_entities[i].state==ABOUT_TO_TRANSFER)
                    {
                        post_msg(client_entities[i].conn_fd,150,NULL);
                        int tran_fd=AcceptConnection(client_entities[i].tran_fd);
                        if (tran_fd==-1)
                        {
                            post_msg(client_entities[i].conn_fd,425,NULL);
                            continue;
                        }
                        FD_SET(tran_fd,&read_set);
                        client_entities[i].tran_fd=tran_fd;
                        client_entities[i].state=TRANSFER;                    }
                    else if(client_entities[i].state==TRANSFER)
                    {
                        if(client_entities[i].list==NOT_LIST)
                        {
                            FILE* file=fopen(client_entities[i].filepath,"rb+");
                            printf("%s\n",client_entities[i].filepath);
                            if(file==NULL)
                            {
                                post_msg(client_entities[i].conn_fd,451,NULL);
                            }
                            else
                            {
                                printf("send_file\n");
                                send_file(&client_entities[i],file,buf);
                                fclose(file);
                            }
                        }
                        else
                        {
                            send_list(&client_entities[i],buf);//todo 
                        }
                    }
                }
                else if(client_entities[i].tran_mode==PORT)
                {
                    // printf("get port client\n");
                    // printf("%d\n",client_entities[i].state);
                    // return 0;
                    if(client_entities[i].state==ABOUT_TO_TRANSFER)
                    {
                        if(connect(client_entities[i].tran_fd, (struct sockaddr *)&client_entities[i].addr, sizeof(client_entities[i].addr))==-1)
                        {
                            close(client_entities[i].tran_fd);
                            printf("port connect error\n");
                            post_msg(client_entities[i].conn_fd,425,NULL);
                        }
                        else
                        {
                            client_entities[i].state=TRANSFER;
                        }
                    }
                    else if(client_entities[i].state==TRANSFER)
                    {
                        if(client_entities[i].list==NOT_LIST)
                        {
                            //printf("PORT send\n");
                            post_msg(client_entities[i].conn_fd,150,NULL);
                            FILE* file=fopen(client_entities[i].filepath,"rb+");
                            printf("%s\n",client_entities[i].filepath);
                            if(file==NULL)
                            {
                                post_msg(client_entities[i].conn_fd,451,NULL);
                            }
                            else
                            {
                                printf("send_file\n");
                                // fseek(file,client_entities[i].offset,SEEK_SET);
                                send_file(&client_entities[i],file,buf);
                                fclose(file);
                            }
                        }
                        else
                        {
                            send_list(&client_entities[i],buf);//todo 
                        }
                    }
                }
                // else
                // {
                //     FD_CLR(client_entities[i].tran_fd,&read_set);
                //     FD_CLR(client_entities[i].tran_fd,&write_set);
                //     client_entities[i].tran_fd=-1;
                //     client_entities[i].tran_mode=NOT_SET;
                // }
            }
        }

        // //榨干socket传来的内容
        // p = 0;
        // char *msg[256];
        // while (1) {
        //     int n = read(conn_fd, sentence + p, 8191 - p);
        //     if (n < 0) {
        //         printf("Error read(): %s(%d)\n", strerror(errno), errno);
        //         close(conn_fd);
        //         continue;
        //     } else if (n == 0) {
        //         break;
        //     } else {
        //         p += n;
        //         if (sentence[p - 1] == '\n') {
        //             break;
        //         }
        //     }
        // }
        // //socket接收到的字符串并不会添加'\0'
        // sentence[p - 1] = '\0';
        // len = p - 1;
        
        // //字符串处理
        // for (p = 0; p < len; p++) {
        //     sentence[p] = toupper(sentence[p]);
        // }

        // //发送字符串到socket
        //  p = 0;
        // while (p < len) {
        //     int n = write(conn_fd, sentence + p, len + 1 - p);
        //     if (n < 0) {
        //         printf("Error write(): %s(%d)\n", strerror(errno), errno);
        //         return 1;
        //      } else {
        //         p += n;
        //     }			
        // }

        // close(conn_fd);
    }
    close(listen_fd);
}

