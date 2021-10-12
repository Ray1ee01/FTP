#include "message.h"

void get_msg(int fd, char *msg, int max_len)
{
    int p = 0;
    while (1) {
        int n = read(fd, msg + p,max_len - p);
        if (n < 0) {
            // printf("Error read(): %s(%d)\n", strerror(errno), errno);
            // close(conn_fd);
            continue;
        } else if (n == 0) {
            break;
        } else {
            p += n;
            if (msg[p - 1] == '\n') {
                break;
            }
        }
    }
    msg[p-1]='\0' // add \0
}


// http://www.tcpipguide.com/free/t_FTPRepliesReplyCodeFormatandImportantReplyCodes-4.htm
void post_msg(int fd,int code, char *msg)
{
    char msg[128];
    switch(code)
    {
    default:
        sprintf(msg,"%d,%s\r\n",code,msg);
        break;
    }
    send(fd,msg,len(msg),MSG_WAITALL);
    return;
}