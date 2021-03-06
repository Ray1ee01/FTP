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
int get_data(int fd, char *msg, int max_len)
{
    if(fd==-1)return -1;
    int p = 0;
    while (1) {
        int n = read(fd, msg + p,max_len - p);
        if (n < 0) {
            // printf("Error read(): %s(%d)\n", strerror(errno), errno);
            // close(conn_fd);
            p=-1;
            continue;
        } else if (n==0) {
            break;
        } else {
            p += n;
        }
    }
    return p;
}

int post_data(int fd, char *msg, int max_len)
{
    if(fd==-1)return -1;
    int p = 0;
    while (1) {
        int n = write(fd, msg + p,max_len - p);
        if (n < 0) {
            // printf("Error read(): %s(%d)\n", strerror(errno), errno);
            // close(conn_fd);
            return -1;
        } else if (n == 0) {
            break;
        } else {
            p+=n;
        }
    }
    return 0;
}

// http://www.tcpipguide.com/free/t_FTPRepliesReplyCodeFormatandImportantReplyCodes-4.htm
void post_msg(int fd,int code, char *pattern)
{
    char msg[256];
    char typical[128];
    switch(code)
    {
    case 125:
        strcpy(typical,"Data connection already open;transfer starting");
        break;
    case 150:
        strcpy(typical,"File status okay; about to open data connection");
        break;
    case 200:
        strcpy(typical,"Type set to I.");
        break;
    case 212:
        strcpy(typical,"Directory status");
        break;
    case 213:
        strcpy(typical,"File status");
        break;
    case 215:
        strcpy(typical,"UNIX Type: L8");
        break;
    case 220:
        strcpy(typical,"Servive ready for new user");
        break;
    case 221:
        strcpy(typical,"goodbye!");
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
    case 331:
        strcpy(typical,"Username OK,need password");
        break;
    case 332:
        strcpy(typical,"Need account for login");
        break;
    case 425:
        strcpy(typical,"Can't open data connection");
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
    case 550:
        strcpy(typical,"Requested action not taken. File unavailable.");
        break;
    default:
        memset(typical,0,sizeof(typical));
        //typical=NULL;
        // strcpy(typical,NULL);
        break;
    }
    if(pattern!=NULL)
    {
        sprintf(msg,"%d %s %s\r\n",code,typical,pattern);
    }
    else
    {
        if(strlen(typical)==0)
        {
            sprintf(msg,"%d %s\r\n",code,pattern);
        }
        else
        {
            sprintf(msg,"%d %s\r\n",code,typical);
        }
    }
    send(fd,msg,strlen(msg),MSG_WAITALL);// why send duplicate last msg?
    // printf("Send msg %s\n",msg);
    // printf("Send msg %s\n",pattern);
    return;
}

// https://blog.csdn.net/lisonglisonglisong/article/details/22699675
// https://www.runoob.com/cprogramming/c-function-fseek.html
void* send_file(void *args)
{
    Client* client=(Client*)args;
    FILE *file;
    if((file=fopen(client->filepath,"rb+"))==NULL)
    {
        post_msg(client->conn_fd,550,NULL);
    }
    else
    {
        fseek(file,client->offset,SEEK_SET);
        char buf[BUFFER_SIZE];
        bzero(buf,BUFFER_SIZE);
        int len=0;
        int flag=0;
        while((len=fread(buf,1,BUFFER_SIZE,file))>0)
        {
            if((flag=post_data(client->tran_fd,buf,len))==-1)
            {
                post_msg(client->conn_fd,426,NULL);
                break;
            }
            client->offset+=len;
            bzero(buf,BUFFER_SIZE);
        }
        if(flag!=-1)
        {
            client->offset=0;
        }
        fclose(file);
        if(client->tran_mode==PASV)
        {
            // printf("PASV mode\n");
            close(client->listen_fd);
            client->listen_fd=-1;
        }
        client->tran_mode=NOT_SET;
        close(client->tran_fd);
        client->tran_fd=-1;
        post_msg(client->conn_fd,226,NULL);
    }
    return 0;
}

void* recv_file(void *args)
{
    Client* client=(Client*)args;
    FILE *file;
    // printf("recv %s\n",client->filepath);
    if((file=fopen(client->filepath,"ab+"))==NULL)
    {
        post_msg(client->conn_fd,550,NULL);
    }
    else
    {
        fseek(file,client->offset,SEEK_SET);
        char buf[BUFFER_SIZE];
        bzero(buf,BUFFER_SIZE);
        int len=0;
        while((len=get_data(client->tran_fd,buf,BUFFER_SIZE))>0)
        {
            // printf("%d\n",client->tran_fd);
            if((fwrite(buf,sizeof(char),len,file))<len)
            {
                post_msg(client->conn_fd,451,NULL);
                break;
            }
            // printf("%s\n",buf);
            client->offset+=len;
            bzero(buf,BUFFER_SIZE);
        }
        fflush(file);
        fclose(file);
        // printf("len %d\n",len);
        if(client->tran_mode==PASV)
        {
            close(client->listen_fd);
            client->listen_fd=-1;
        }
        client->tran_mode=NOT_SET;
        if(len==-1)
        {
            post_msg(client->conn_fd,426,NULL);
        }
        else
        {
            client->offset=0;
        }
        close(client->tran_fd);
        client->tran_fd=-1;
        post_msg(client->conn_fd,226,NULL);
    }
    return 0;
}

//https://blog.csdn.net/clarkness/article/details/88721769
void* send_list(void* args)
{
    // printf("send_list\n");
    Client* client=(Client*)args;
    FILE *file=NULL;
    char cmd[256]="ls -l ";
    sprintf(cmd,"ls -l %s",client->curdir);
    // printf("cmd %s",cmd);
    char buf[BUFFER_SIZE];
    bzero(buf,BUFFER_SIZE);
    file = popen(cmd,"r");
    if(file!=NULL)
    {
        int len=0;
        while((len=fread(buf,sizeof(char),BUFFER_SIZE,file))>0) // ?????????
        {
            // printf("buf:%s\n",buf);
            if(post_data(client->tran_fd,buf,len)==-1) // to do reliable send
            {
                post_msg(client->conn_fd,426,NULL);
                break;
            }
            bzero(buf,BUFFER_SIZE);
        }
    }
    pclose(file);

    client->tran_mode=NOT_SET;
    close(client->tran_fd);
    close(client->listen_fd);
    client->tran_fd=-1;
    client->listen_fd=-1;
    post_msg(client->conn_fd,226,NULL);
    return 0;
}