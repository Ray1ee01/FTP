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
#define RNFR 2
#define RNFO 3
#define DOWNLOAD 4
#define UPLOAD 5

// list
#define NOT_LIST -1
#define LIST 0

// constant for FTP system
#define MAX_CLIENTS 10


#define BUFFER_SIZE 100

typedef struct
{
    struct sockaddr_in addr; // 只在port中使用
    int conn_fd;             // 传递command用fd
    int tran_fd;             // 文件transfer用fd
    int listen_fd;           // PASV only
    int login;               // 登录状态 
    int tran_mode;           // 建立transfer连接的模式 PASV or POST 
    int state;               // 是否在处理命令
    char filepath[128];      // 完整的文件路径 root+params
    char curdir[128];        // 管理路径
    char rnfile[128];        // rename文件
    int offset;              // 文件传输初始偏移量
    int list;
    // todo
}Client;
Client client_entities[MAX_CLIENTS];

char server_ip[16];
char root[128];

fd_set read_set,write_set;
#endif