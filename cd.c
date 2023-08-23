#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

void split_command(char *input, char *command, char *arguments[]) {
    int arg_index = 0;
    char *token = input;

    while (*input != '\0' && arg_index < MAX_ARGUMENTS - 1) {
        if (*input == ' ') {
            *input = '\0';
            arguments[arg_index++] = token;
            token = input + 1;
        }
        input++;
    }

    arguments[arg_index++] = token;
    arguments[arg_index] = NULL;
    strcpy(command, arguments[0]);
}

void update_pwd(char *new_pwd) {
    if (setenv("PWD", new_pwd, 1) != 0) {
        perror("setenv");
    }
}

int main() {
    char input[MAX_COMMAND_LENGTH];
    char command[MAX_COMMAND_LENGTH];
    char *arguments[MAX_ARGUMENTS];
    
    while (1) {
        printf("$ ");
        
        if (fgets(input, MAX_COMMAND_LENGTH, stdin) == NULL) {
            // Handle Ctrl+D (EOF)
            printf("\nGoodbye!\n");
            break;
        }
        
        // Remove the trailing newline character
        input[strcspn(input, "\n")] = '\0';

        split_command(input, command, arguments);
        
        // Implement the "exit" built-in command
        if (strcmp(command, "exit") == 0) {
            int exit_status = 0;
            if (arguments[1]) {
                exit_status = atoi(arguments[1]);
            }
            printf("Exiting the shell with status %d\n", exit_status);
            exit(exit_status);
        }
        
        // Implement the "setenv" built-in command
        if (strcmp(command, "setenv") == 0) {
            if (arguments[1] && arguments[2]) {
                if (setenv(arguments[1], arguments[2], 1) != 0) {
                    perror("setenv");
                }
            } else {
                fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
            }
            continue;
        }
        
        // Implement the "unsetenv" built-in command
        if (strcmp(command, "unsetenv") == 0) {
            if (arguments[1]) {
                if (unsetenv(arguments[1]) != 0) {
                    perror("unsetenv");
                }
            } else {
                fprintf(stderr, "Usage: unsetenv VARIABLE\n");
            }
            continue;
        }
        
        // Implement the "cd" built-in command
        if (strcmp(command, "cd") == 0) {
            char *new_dir = arguments[1];
            if (!new_dir) {
                new_dir = getenv("HOME");
            } else if (strcmp(new_dir, "-") == 0) {
                new_dir = getenv("OLDPWD");
            }
            
            char current_dir[MAX_COMMAND_LENGTH];
            getcwd(current_dir, sizeof(current_dir));
            if (chdir(new_dir) != 0) {
                perror("chdir");
            } else {
                update_pwd(current_dir);
                update_pwd(getcwd(NULL, 0));
            }
            continue;
        }
        
        // Execute the command using execvp() if it exists
        if (access(command, X_OK) == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                execvp(command, arguments);
                perror("execvp");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                waitpid(pid, NULL, 0);
            } else {
                perror("fork");
            }
        } else {
            printf("Command not found: %s\n", command);
        }
    }
    
    return 0;
}

