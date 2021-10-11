// handle all commands

#ifndef COMMAND_UTILS
#define COMMAND_UTILS
struct CommandFilter
{
    char* name;
    void (*fptr)(const char* params);
};

void HandleCommand(const char *cmd,const char *params);

void CmdABOR(const char *params);

void CmdUSER(const char *params);

void CmdPASS(const char *params);

void CmdRETR(const char *params);

void CmdSTOR(const char *params);

void CmdQUIT(const char *params);

void CmdSYST(const char *params);

void CmdTYPE(const char *params);

void CmdPORT(const char *params);

void CmdPASV(const char *params);

void CmdMKD(const char *params);

void CmdCWD(const char *params);

void CmdPWD(const char *params);

void CmdLIST(const char *params);

void CmdRMD(const char *params);

void CmdRNFR(const char *params);

void CmdRNTO(const char *params);



#endif