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
    msg[p-1]='\0'; // add \0
}


// http://www.tcpipguide.com/free/t_FTPRepliesReplyCodeFormatandImportantReplyCodes-4.htm
void post_msg(int fd,int code, char *pattern)
{
    char msg[128];
    char typical[64];
    switch(code)
    {
    case 125:
        strcpy(typical,"Data connection already open;transfer starting");
        break;
    case 150:
        strcpy(typical,"File status okay; about to open data connection");
        break;
    case 200:
        strcpy(typical,NULL);
        break;
    case 212:
        strcpy(typical,"Directory status");
        break;
    case 213:
        strcpy(typical,"File status");
        break;
    case 215:
        strcpy(typical,"system type");
        break;
    case 220:
        strcpy(typical,"Servive ready for new user");
        break;
    case 221:
        strcpy(typical,"Service closing control connection");
        break;
    case 225:
        strcpy(typical,"Data connection open; no transfer in progress");
        break;
    case 226:
        strcpy(typical,"Closing data connection");
        break;
    case 227:
        strcpy(typical,"Entering Passive Mode");
        break;
    case 230:
        strcpy(typical,"User logged in, proceed");
        break;
    case 250:
        strcpy(typical,"Requested file action, completed");
        break;
    case 257:
        strcpy(typical,"created");
        break;
    case 332:
        strcpy(typical,"Need account for login");
        break;
    case 426:
        strcpy(typical,"Connection closed; transfer aborted");
        break;
    case 450:
        strcpy(typical,"Requested file action not taken.File unavailable");
        break;
    case 500:
        strcpy(typical,"Syntax error,command unrecognized.");
        break;
    case 501:
        strcpy(typical,"Syntax error in parameters or arguments.");
        break;
    case 503:
        strcpy(typical,"Bad sequence of commands.");
        break;
    case 504:
        strcpy(typical,"Command not implemented for that parameter.");
        break;
    case 530:
        strcpy(typical,"Not logged in.");
        break;
    default:
        strcpy(typical,NULL);
        break;
    }
    sprintf(msg,"%d,%s %s\r\n",code,typical,pattern);
    send(fd,msg,strlen(msg),MSG_WAITALL);
    return;
}