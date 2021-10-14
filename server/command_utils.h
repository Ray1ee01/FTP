// handle all commands

#ifndef COMMAND_UTILS
#define COMMAND_UTILS

#include "global.h"

typedef struct
{
    char* name;
    void (*fptr)(const char* params,Client *client);
}CommandFilter;


void HandleCommand(const char *cmd,const char *params,Client *client);

void CmdABOR(const char *params,Client *client);

void CmdUSER(const char *params,Client *client);

void CmdPASS(const char *params,Client *client);

void CmdRETR(const char *params,Client *client);

void CmdSTOR(const char *params,Client *client);

void CmdQUIT(const char *params,Client *client);

void CmdSYST(const char *params,Client *client);

void CmdTYPE(const char *params,Client *client);

void CmdPORT(const char *params,Client *client);

void CmdPASV(const char *params,Client *client);

void CmdMKD(const char *params,Client *client);

void CmdCWD(const char *params,Client *client);

void CmdPWD(const char *params,Client *client);

void CmdLIST(const char *params,Client *client);

void CmdRMD(const char *params,Client *client);

void CmdRNFR(const char *params,Client *client);

void CmdRNTO(const char *params,Client *client);




#endif