#include <stdio.h>

#include "exec.h"
#include "utils.h"

void msh_load_config() {}

void msh_loop() {
    char *line;
    char **args;
    int status;

    do {
        msh_print_prompt();
        line = msh_read_line();

        if (line[0] == EOF) {
            printf("\n");
            break;
        }

        args = msh_split_line(line);
        struct command_ctx ctx =  msh_process_args(args);
        status = msh_execute(&ctx);

        free(line);
        free(args);
    } while (status);
}

void msh_cleanup() {}

int main(int argc, char **argv) {
    msh_load_config();
    msh_loop();
    msh_cleanup();

    return 0;
}
