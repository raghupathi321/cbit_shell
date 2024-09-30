#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>

void signal_handler(int signum) {
    if (signum == SIGINT) {
        printf("\nExiting shell...\n");
        exit(0);
    }
}

void execute_command(char *command) {
    char *args[100];
    char *token = strtok(command, " ");
    int i = 0;

    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (execvp(args[0], args) == -1) {
        perror("Error");
    }
}

void handle_env() {
    extern char **environ;
    for (char **env = environ; *env; ++env) {
        printf("%s\n", *env);
    }
}

void set_environment_variable(char *var, char *value) {
    if (setenv(var, value, 1) == -1) {
        perror("setenv error");
    }
}

void unset_environment_variable(char *var) {
    if (unsetenv(var) == -1) {
        perror("unsetenv error");
    }
}

void print_help() {
    printf("Available commands:\n");
    printf("exit                   - Exit the shell\n");
    printf("env                    - Print environment variables\n");
    printf("setenv VAR VALUE       - Set an environment variable\n");
    printf("unsetenv VAR           - Unset an environment variable\n");
    printf("pwd                    - Print current working directory\n");
    printf("whoami                 - Print current user name\n");
    printf("echo TEXT              - Print TEXT to the terminal\n");
    printf("ls                     - List directory contents\n");
    printf("cd DIRECTORY           - Change current directory\n");
    printf("help                   - Display this help menu\n");
    printf("touch FILE             - Create an empty file\n");
    printf("del FILE               - Delete a file\n");
    printf("cat FILE               - Display the contents of a file\n");
}

void change_directory(char *path) {
    if (chdir(path) == -1) {
        perror("cd error");
    }
}

void list_directory() {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(".")) == NULL) {
        perror("opendir error");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

void create_file(char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fclose(file);
        printf("File '%s' created successfully.\n", filename);
    } else {
        perror("File creation error");
    }
}

void delete_file(char *filename) {
    if (remove(filename) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        perror("File deletion error");
    }
}

void display_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        char ch;
        while ((ch = fgetc(file)) != EOF) {
            putchar(ch);
        }
        fclose(file);
    } else {
        perror("File read error");
    }
}

int main() {
    signal(SIGINT, signal_handler);
    char command[256];

    while (1) {
        printf("$cbit: ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; // EOF condition
        }

        command[strcspn(command, "\n")] = 0; // Remove newline character

        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "env") == 0) {
            handle_env();
        } else if (strncmp(command, "setenv ", 7) == 0) {
            char *var = strtok(command + 7, " ");
            char *value = strtok(NULL, " ");
            if (var && value) {
                set_environment_variable(var, value);
            } else {
                printf("Usage: setenv VAR VALUE\n");
            }
        } else if (strncmp(command, "unsetenv ", 9) == 0) {
            char *var = command + 9;
            unset_environment_variable(var);
        } else if (strcmp(command, "pwd") == 0) {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("getcwd error");
            }
        } else if (strcmp(command, "whoami") == 0) {
            char *username = getlogin();
            if (username) {
                printf("%s\n", username);
            } else {
                perror("whoami error");
            }
        } else if (strncmp(command, "echo ", 5) == 0) {
            printf("%s\n", command + 5);
        } else if (strcmp(command, "help") == 0) {
            print_help();
        } else if (strncmp(command, "cd ", 3) == 0) {
            change_directory(command + 3);
        } else if (strcmp(command, "ls") == 0) {
            list_directory();
        } else if (strncmp(command, "touch ", 6) == 0) {
            create_file(command + 6);
        } else if (strncmp(command, "del ", 4) == 0) {
            delete_file(command + 4);
        } else if (strncmp(command, "cat ", 4) == 0) {
            display_file(command + 4);
        } else {
            pid_t pid = fork();
            if (pid == 0) {
                execute_command(command);
                exit(0);
            } else if (pid > 0) {
                wait(NULL);
            } else {
                perror("Fork failed");
            }
        }
    }
    return 0;
}
