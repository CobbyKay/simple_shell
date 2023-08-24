#ifndef SHELL_H
#define SHELL_H

void split_command(char *input, char *command, char *arguments[]);
void update_pwd(char *new_pwd);
void execute_external_command(char *command, char *arguments[]);
void handle_builtin_commands(char *command, char *arguments[]);

#endif /* SHELL_H */

