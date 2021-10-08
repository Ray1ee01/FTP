// handle all commands

#ifndef COMMAND_UTILS
#define COMMAND_UTILS
struct CommandFilter
{
    char* name;
    void (*fptr)(const char* params);
};

void HandleCommand(const char *cmd,const char *params);



#endif