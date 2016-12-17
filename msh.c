#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MSH_READLINE_BUFSIZE 1024
#define MSH_TOK_BUFSIZE 64
#define MSH_TOK_DELIM "\t\r\a\n"

void msh_fail(char *message) {
    fprintf(stderr, "msh: %s\n", message);
    exit(1);
}

char *msh_read_line() {
    int bufsize = MSH_READLINE_BUFSIZE;
    char *buffer = calloc(bufsize, sizeof(char));

    if (!buffer) {
        msh_fail("buffer calloc error");
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

char **msh_split_line(char *line) {
    int bufsize = MSH_TOK_BUFSIZE;
    char **tokens = calloc(bufsize, sizeof(char*));
    if (!tokens) {
        msh_fail("buffer calloc error");
    }

    int position = 0;
    char *token = strtok(line, MSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                msh_fail("buffer realloc error");
            }
        }

        token = strtok(NULL, MSH_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

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
