#ifndef _COMMANDS_H
#define _COMMANDS_H

typedef int (*command)(char **);

struct builtin_cmd {
    char *name;
    command cmd;
};

int msh_cd(char **args);
int msh_help(char **args);
int msh_exit(char **args);

int msh_num_builtins();

extern struct builtin_cmd builtins[];

#endif
