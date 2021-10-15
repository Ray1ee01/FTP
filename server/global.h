/*
全局变量/类型 以及宏定义
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <linux/if.h>
#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <getopt.h>
#include <dirent.h>

// login
#define NOT_LOGIN -1
#define NEED_PASS 0
#define LOGIN 1

// tran_mode
#define NOT_SET -1
#define PORT 0
#define PASV 1

// state
#define NOT_SET -1
#define ABOUT_TO_TRANSFER 0
#define TRANSFER 1


// constant for FTP system
#define MAX_CLIENTS 64


#define BUFFER_SIZE 128

typedef struct
{
    struct sockaddr_in addr; // 只在port中使用
    int conn_fd;             // 传递command用fd
    int tran_fd;             // 文件transfer用fd
    int login;               // 登录状态 
    int tran_mode;           // 建立transfer连接的模式 PASV or POST 
    int state;               // 是否在处理命令
    char filepath[128];      // 完整的文件路径 root+params
    int offset;              // 文件传输初始偏移量
    // todo
}Client;
Client client_entities[MAX_CLIENTS];

char server_ip[16];
char root[16];

fd_set read_set,write_set;
#endif