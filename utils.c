#include "utils.h"

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
