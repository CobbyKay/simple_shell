#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

int main() {
    char input[MAX_COMMAND_LENGTH];
    char *command;
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
        
        // Tokenize the input to separate command and arguments
        char *token = strtok(input, " ");
        if (token == NULL) {
            continue; // Empty line, prompt again
        }
        
        command = token;
        
        // Implement the "exit" built-in command
        if (strcmp(command, "exit") == 0) {
            printf("Exiting the shell\n");
            break;
        }
        
        int arg_index = 0;
        
        while (token != NULL && arg_index < MAX_ARGUMENTS - 1) {
            token = strtok(NULL, " ");
            arguments[arg_index] = token;
            arg_index++;
        }
        arguments[arg_index] = NULL; // Null-terminate the arguments array
        
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

