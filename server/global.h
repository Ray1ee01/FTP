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


#define NOT_LOGIN -1
#define NEED_PASS 0
#define LOGIN 1



struct Client
{
    struct sockaddr_in addr; // 只在port中使用
    int conn_fd;             // 传递command用fd
    int tran_fd;             // 文件transfer用fd
    int login;               // 登录状态 
    int tran_mode;           // 建立transfer连接的模式 PASV or POST 
    int state;               // 是否在处理命令
    // todo
}client_entities[2];


#endif