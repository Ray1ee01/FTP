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
        }
    }
    return p;
}

int post_data(int fd, char *msg, int max_len)
{
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
        sprintf(msg,"%d %s\r\n",code,typical);
    }
    // printf("Send msg %s\n",typical);
    // printf("Send msg %s\n",pattern);
    send(fd,msg,strlen(msg),0);// why send duplicate last msg?
    return;
}

// https://blog.csdn.net/lisonglisonglisong/article/details/22699675
// https://www.runoob.com/cprogramming/c-function-fseek.html
int send_file(Client *client, FILE *file, char *buf)
{
    bzero(buf,BUFFER_SIZE);
    int len=0;
    while((len=fread(buf,sizeof(char),BUFFER_SIZE,file))>0) // 括号！
    {
        if(client->state!=TRANSFER)
        {
            return -1; // state conflict
        }
        else
        {
            printf("len:%d,buf:%s\n",len,buf);
            if(post_data(client->tran_fd,buf,len)==-1) // to do reliable send
            {
                printf("Send:%s Failed./n",buf);
                client->state=ABOUT_TO_TRANSFER;
                return 0; // send fail
            }
            client->offset+=len;
            bzero(buf,BUFFER_SIZE);
        }
    }
    fclose(file);
    printf("File transfer end\n");
    client->state=NOT_SET;
    FD_CLR(client->tran_fd,&write_set);
    client->offset=0;
    return 1; // success
}

int recv_file(Client *client, FILE *file, char *buf)
{
    bzero(buf,BUFFER_SIZE);
    int len=0;
    if(client->state!=TRANSFER)
    {
        return -1;
    }
    else
    {
        while((len=get_data(client->tran_fd,buf,BUFFER_SIZE))>0)
        {
            printf("buffer: %s",buf);
            if((len=fwrite(buf,sizeof(char),BUFFER_SIZE,file))<0)
            {
                printf("Recv:%s Failed./n",buf);
                client->state=ABOUT_TO_TRANSFER;
                return 0; // send fail
            }
            client->offset+=len;
            bzero(buf,BUFFER_SIZE);
        }
        fclose(file);
        printf("File transfer end\n");
        client->state=NOT_SET;
        FD_CLR(client->tran_fd,&write_set);
        FD_CLR(client->tran_fd,&read_set);
        client->offset=0;
        return 1; // success
    }
}

//https://blog.csdn.net/clarkness/article/details/88721769
int send_list(Client* client,char* buf)
{
    FILE *file=NULL;
    char cmd[256];
    sprintf(cmd,"ls -l %s",client->filepath);
    file = popen(cmd,'r');
    fgets(buf,sizeof(buf),file);
    printf("%s",buf);
    pclose(file);
}