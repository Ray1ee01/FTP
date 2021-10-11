#include "command_utils.h"
#include "global.h"
struct CommandFilter[3];

CommandFilter[0]={
    "ABOR",
    CmdABOR
};
CommandFilter[1]={
    "USER",
    CmdUSER
};
CommandFilter[2]={
    "PASS",
    CmdPASS
};
void HandleCommand(const char* cmd,const char* params)
{
    for (int i=0;i<3;i++)
    {
        if (strcmp(cmd,CommandFilter[i].name)==0)
        {
            CommandFilter[i].fptr(params);
            break;
        }
    }
}
void CmdABOR(const char* params)
{

}

void CmdUSER(const char* params)
{
    
}

