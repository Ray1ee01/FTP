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
void post_msg(int fd,int code, char *pattern)
{
    char msg[128];
    char typical[64];
    switch(code)
    {
    case 125:
        typical="Data connection already open;transfer starting";
        break;
    case 150:
        typical="File status okay; about to open data connection";
        break;
    case 200:
        typical=NULL;
        break;
    case 212:
        typical="Directory status";
        break;
    case 213:
        typical="File status";
        break;
    case 215:
        typical="system type";
        break;
    case 220:
        typical="Servive ready for new user";
        break;
    case 221:
        typical="Service closing control connection";
        break;
    case 225:
        typical="Data connection open; no transfer in progress";
        break;
    case 226:
        typical="Closing data connection";
        break;
    case 227:
        typical="Entering Passive Mode";
        break;
    case 230:
        typical="User logged in, proceed";
        break;
    case 250:
        typical="Requested file action, completed";
        break;
    case 257:
        typical="created";
        break;
    case 332:
        typical="Need account for login";
        break;
    case 426:
        typical="Connection closed; transfer aborted";
        break;
    case 450:
        typical="Requested file action not taken.File unavailable";
        break;
    case 500:
        typical="Syntax error,command unrecognized.";
        break;
    case 501:
        typical="Syntax error in parameters or arguments.";
        break;
    case 503:
        typical="Bad sequence of commands.";
        break;
    case 504:
        typical="Command not implemented for that parameter.";
        break;
    case 530:
        typical="Not logged in.";
        break;
    default:
        typical=NULL;
        break;
    }
    sprintf(msg,"%d,%s %s\r\n",code,typical,pattern);
    send(fd,msg,len(msg),MSG_WAITALL);
    return;
}