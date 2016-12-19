#ifndef _COMMON_H
#define _COMMON_H

struct command_ctx {
    char **args;
    char *stdout_file;
    char *stderr_file;
    char *stdin_file;
};

#endif
