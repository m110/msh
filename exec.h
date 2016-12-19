#ifndef _EXEC_H
#define _EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "commands.h"

int msh_launch(char **args);
int msh_execute(char **args);

#endif
