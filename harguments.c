#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10
/*
*execute_command - executes command
*@command: parameter
*@arguments: parameter
*Return: void
*/
void execute_command(char *command, char *arguments[])
{
pid_t pid = fork();

if (pid == 0)
{

execvp(command, arguments);
perror("execvp");
exit(EXIT_FAILURE);
}
else if (pid > 0)
{

waitpid(pid, NULL, 0);
}
else
{
perror("fork");
}
}

int main() 
{
char input[MAX_COMMAND_LENGTH];
char *command;
char *arguments[MAX_ARGUMENTS];

while (1) 
{
printf("$ ");

if (fgets(input, MAX_COMMAND_LENGTH, stdin) == NULL)
{
printf("\nGoodbye!\n");
break;
}

input[strcspn(input, "\n")] = '\0';

char *token = strtok(input, " ");
if (token == NULL) 
{
continue;
}

command = token;
int arg_index = 0;

while (token != NULL && arg_index < MAX_ARGUMENTS - 1)
{
token = strtok(NULL, " ");
arguments[arg_index] = token;
arg_index++;
}
arguments[arg_index] = NULL;

execute_command(command, arguments);
}

return 0;
}

