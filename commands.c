#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "commands.h"

struct builtin_cmd builtins[] = {
    { "cd",   &msh_cd },
    { "help", &msh_help },
    { "exit", &msh_exit },
};

int msh_num_builtins() {
    return sizeof(builtins) / sizeof(struct builtin_cmd);
}

int msh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "msh: cd expects an argument\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("msh");
        }
    }

    return 1;
}

int msh_help(char **args) {
    printf("msh shell by m110\n");

    printf("Builtin commands:\n");
    for (int i = 0; i < msh_num_builtins(); i++) {
        printf("\t%s\n", builtins[i].name);
    }

    return 1;
}

int msh_exit(char **args) {
    return 0;
}
