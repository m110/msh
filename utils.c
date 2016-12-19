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

struct command_ctx msh_process_args(char **args) {
    char *stdin_file = NULL;
    char *stderr_file = NULL;
    char *stdout_file = NULL;

    int bufsize = MSH_TOK_BUFSIZE;
    char **new_args = calloc(bufsize, sizeof(char*));

    int position = 0;
    int new_position = 0;

    while (args[position] != NULL) {
        if (args[position][0] == MSH_STDOUT_SYMBOL) {
            stdout_file = args[position] + 1;
            position++;
            continue;
        }

        if (args[position][0] == MSH_STDIN_SYMBOL) {
            stdin_file = args[position] + 1;
            position++;
            continue;
        }

        new_args[new_position] = args[position];
        position++;
        new_position++;
    }

    struct command_ctx ctx = {
        .args = new_args,
        .stdin_file = stdin_file,
        .stderr_file = stderr_file,
        .stdout_file = stdout_file,
    };
    return ctx;
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
