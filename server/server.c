



#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <getopt.h>

#include "socket_utils.h"
#include "global.h"


int main(int argc, char **argv) {
    int listen_fd, conn_fd;		//监听socket和连接socket不一样，后者用于数据传输
    struct sockaddr_in addr;
    char sentence[8192];
    int p;
    int len;
    char root[128]="/FTPtemp";
    int port =6789;
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
            printf("Port: %s.\n",optarg);
            // TODO
            // 检测port是否可用 or 合法
            port=atoi(optarg);
            break;
        case 'r':
            printf("Directory: %s.\n",optarg);
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
    listen_fd = ListenBind(port);
    
    //持续监听连接请求
    while (1) {
        conn_fd=AcceptConnection(listen_fd);
        if (conn_fd==-1)continue;
        // todo handle request
        //榨干socket传来的内容
        p = 0;
        while (1) {
            int n = read(conn_fd, sentence + p, 8191 - p);
            if (n < 0) {
                printf("Error read(): %s(%d)\n", strerror(errno), errno);
                close(conn_fd);
                continue;
            } else if (n == 0) {
                break;
            } else {
                p += n;
                if (sentence[p - 1] == '\n') {
                    break;
                }
            }
        }
        //socket接收到的字符串并不会添加'\0'
        sentence[p - 1] = '\0';
        len = p - 1;
        
        //字符串处理
        for (p = 0; p < len; p++) {
            sentence[p] = toupper(sentence[p]);
        }

        //发送字符串到socket
         p = 0;
        while (p < len) {
            int n = write(conn_fd, sentence + p, len + 1 - p);
            if (n < 0) {
                printf("Error write(): %s(%d)\n", strerror(errno), errno);
                return 1;
             } else {
                p += n;
            }			
        }

        close(conn_fd);
    }
    close(listen_fd);
}

