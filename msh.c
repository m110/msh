#include <stdio.h>
#include <stdlib.h>

#define MSH_READLINE_BUFSIZE 1024

void msh_fail(char *message) {
    fprintf(stderr, "msh: %s\n", message);
    exit(1);
}

char *msh_read_line() {
    int bufsize = MSH_READLINE_BUFSIZE;
    char *buffer = malloc(sizeof(char) * bufsize);

    if (!buffer) {
        msh_fail("buffer malloc error");
    }

    int position = 0;
    while (1) {
        int c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            break;
        }

        buffer[position] = c;
        position++;

        // Reallocate the buffer
        if (position >= bufsize) {
            bufsize += MSH_READLINE_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                msh_fail("buffer realloc error");
            }
        }
    }

    return buffer;
}

void msh_load_config() {}

void msh_loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("msh$ ");
        line = msh_read_line();

        free(line);
    } while (status);
}

void msh_cleanup() {}

int main(int argc, char **argv) {
    msh_load_config();
    msh_loop();
    msh_cleanup();

    return 0;
}
