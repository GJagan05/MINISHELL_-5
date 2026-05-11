#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUILTIN  1
#define EXTERNAL 2
#define NO_COMMAND 0

// Global variables 
extern pid_t pid;
extern int status;
extern char input_string[100];
extern char *external_commands[200];

// Linked list for jobs 
typedef struct node
{
    pid_t pid;
    char cmd[100];
    struct node *link;
} NODE;

extern NODE *head;

// Function declarations 
void scan_input(char *prompt, char *input_string);
char *get_command(char *input);
int check_command_type(char *command);
void execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);
void insert_at_first(pid_t pid, char *cmd);
void print_jobs(void);
void delete_first(void);

#endif