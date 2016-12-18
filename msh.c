#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#include "commands.h"

#define MSH_READLINE_BUFSIZE 1024

#define MSH_TOK_BUFSIZE 64
#define MSH_TOK_DELIM "\t\r\a\n "

#define MSH_CWD_BUFSIZE 128

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

        if (c == '\n') {
            buffer[position] = '\0';
            break;
        }

        buffer[position] = c;
        position++;

        if (c == EOF) {
            break;
        }

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

char *msh_get_username() {
    struct passwd *pw;
    uid_t uid = geteuid();

    pw = getpwuid(uid);
    if (pw == NULL) {
        perror("getpwuid");
        msh_fail("getpwuid error");
    }

    return pw->pw_name;
}

void msh_print_prompt() {
    char suffix = '$';

    char host[MSH_CWD_BUFSIZE];
    if (gethostname(host, MSH_CWD_BUFSIZE) != 0) {
        perror("gethostname");
        msh_fail("gethostname error");
    }

    char *user = msh_get_username();

    char cwd[MSH_CWD_BUFSIZE];
    if (getcwd(cwd, MSH_CWD_BUFSIZE) == NULL) {
        perror("getcwd");
        msh_fail("getcwd error");
    }

    printf("%s@%s %s %c ", user, host, cwd, suffix);
}

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
        status = msh_execute(args);

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
