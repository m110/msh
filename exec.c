#include "exec.h"

int msh_launch(char **args) {
    pid_t pid = fork();

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
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

int msh_execute(char **args) {
    if (args[0] == NULL) {
        return 1;
    }

    for (int i = 0; i < msh_num_builtins(); i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            return (*builtins[i].cmd)(args);
        }
    }

    return msh_launch(args);
}
