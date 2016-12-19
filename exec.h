#ifndef _EXEC_H
#define _EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "commands.h"

int msh_launch(struct command_ctx *ctx);
int msh_execute(struct command_ctx *ctx);

#endif
