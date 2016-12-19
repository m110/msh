#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define MSH_READLINE_BUFSIZE 1024

#define MSH_TOK_BUFSIZE 64
#define MSH_TOK_DELIM "\t\r\a\n "

#define MSH_CWD_BUFSIZE 128

void msh_fail(char *message);
char *msh_read_line();
char **msh_split_line(char *line);
char *msh_get_username();
void msh_print_prompt();

#endif
