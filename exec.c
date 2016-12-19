#include "exec.h"

int msh_launch(struct command_ctx *ctx) {
    pid_t pid = fork();

    if (pid == 0) {
        if (ctx->stdout_file != NULL && !freopen(ctx->stdout_file, "w", stdout)) {
            perror("msh");
            exit(1);
        }

        if (ctx->stdin_file != NULL && !freopen(ctx->stdin_file, "r", stdin)) {
            perror("msh");
            exit(1);
        }

        if (execvp(ctx->args[0], ctx->args) == -1) {
            perror("msh");
        }
        exit(1);
    } else if (pid < 0) {
        perror("msh");
    } else {
        int status;
        do {
            pid_t wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int msh_execute(struct command_ctx *ctx) {
    if (ctx->args[0] == NULL) {
        return 1;
    }

    for (int i = 0; i < msh_num_builtins(); i++) {
        if (strcmp(ctx->args[0], builtins[i].name) == 0) {
            return (*builtins[i].cmd)(ctx->args);
        }
    }

    return msh_launch(ctx);
}
