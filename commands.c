#include"header.h"

NODE *head = NULL;


char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","fg","bg","jobs","NULL"};




char *get_command(char *input)                                 // Function to extract first word (command) from full input
{
    static char cmd[100];                                      // Static buffer to store extracted command

    int i = 0;                                                 // Index counter

                                                               // Copy characters until space or end of string
    while (input[i] != ' ' && input[i] != '\0')
    {
        cmd[i] = input[i];                                     // Store character into cmd array

        i++;                                                   // Move to next character
    }

    cmd[i] = '\0';                                             // Null-terminate the command string

    return cmd;                                                // Return extracted command
}



                                                                // Read external commands from external_cmd.txt file
void extract_external_commands(char **external_commands)
{
    FILE *fp;                                                   // File pointer
    char buffer[100];                                           // Temporary buffer
    int i = 0;                                                  // Index counter

    fp = fopen("external_cmd.txt", "r");                        // Open file in read mode

    if (fp == NULL)                                             // If file not found
    {
       printf("external_cmd.txt not found\n");                  // Print error

       return;                                                  // Exit function
    }

                                                                // Read each line from file
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';                    // Remove newline

        external_commands[i] = malloc(strlen(buffer) + 1);       // Allocate memory

        strcpy(external_commands[i], buffer);                    // Copy command

        i++;                                                     // Move to next index
    }

    external_commands[i] = NULL;                                 // Mark end of array

    fclose(fp);                                                  // Close file
}



                                                                 // Check whether command is BUILTIN or EXTERNAL
int check_command_type(char *cmd)
{
    int i;

                                                                 // Check in built-in commands
    for (i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(cmd, builtins[i]) == 0)                       // Compare strings

            return BUILTIN;                                      // Return built-in type
    }

                                                                 // Check in external commands list
    for (i = 0; external_commands[i] != NULL; i++)
    {
        if (strcmp(cmd, external_commands[i]) == 0)
            return EXTERNAL;                                     // Return external type
    }

    return NO_COMMAND;                                           // If not found
}



                                                                 // Handle signals like Ctrl+C, Ctrl+Z and SIGCHLD
void signal_handler(int signum)
{
    if (signum == SIGINT)                                        // Ctrl+C pressed
    {
        if (pid > 0)                                             // If child running

            kill(pid, SIGINT);                                   // Send SIGINT to child

        else
        {
            printf("\nminishell$: ");                            // Print new prompt

            fflush(stdout);                                      // Flush output buffer
        }
    }
    else if (signum == SIGTSTP)                                  // Ctrl+Z pressed
    {
        if (pid > 0)                                             // If child running
        {
            kill(pid, SIGTSTP);                                  // Stop child

            insert_at_first(pid, input_string);                  // Store in jobs list
        }
        else
        {
            printf("\nminishell$: ");                            //printing the prompt 

            fflush(stdout);
        }
    }
    else if (signum == SIGCHLD)                                  // Background process finished
    {
        int st;
        while (waitpid(-1, &st, WNOHANG) > 0)                    // Non-blocking wait
            printf("\nBackground process finished\n");
    }
}



                                                                 // Execute external commands (with or without pipes)
void execute_external_commands(char *input)
{
    char *args[20];
        int i = 0;
                                                                 // Check if pipe symbol exists
    if (strchr(input, '|') != NULL)
    {
        char *commands[20];                                      // Store individual commands

        int count = 0;                                           // Count number of commands

        commands[count] = strtok(input, "|");                    // Split by pipe
        while (commands[count] != NULL)
        {
            count++;                                             // incrementing the count

            commands[count] = strtok(NULL, "|");                 // finding the pipe using the strtok 
        }

        args[i] = NULL;

    if (execvp(args[0], args) == -1)
    {
        printf("%s: Command not found\n", args[0]);
        exit(1);
    }

        int pipefd[2];                                           // Pipe file descriptors

        int prev_fd = 0;                                         // Store previous pipe read end

        for (int i = 0; i < count; i++)
        {
            pipe(pipefd);                                        // Create pipe

            pid_t child = fork();                                // Create child process

            if (child == 0)                                      // Inside child
            {
                if (i != 0)                                      // If not first command
                {
                    dup2(prev_fd, 0);                            // Read from previous pipe
                    close(prev_fd);
                }

                if (i != count - 1)                              // If not last command

                    dup2(pipefd[1], 1);                          // Write to pipe

                close(pipefd[0]);                                // close pipes read end

                close(pipefd[1]);                                // close pipes write end 

                char *argv[20];                                  // declaring the argument vector as a pointer
                int j = 0;

                argv[j] = strtok(commands[i], " ");              // Split arguments
                while (argv[j] != NULL)
                {
                    j++;
                    argv[j] = strtok(NULL, " ");                 // storing in to the argument vector
                }

                execvp(argv[0], argv);                           // Execute command
                printf("%s: Command not found\n", args[0]);
                exit(1);
            }
            else
            {
                wait(NULL);                                      // Wait for child

                close(pipefd[1]);                                // Close write end

                prev_fd = pipefd[0];                             // Save read end
            }
        }
    }
    else   
    {
        char *argv[20];                                          
        int i = 0;                                                // intilizing the variable i 

        argv[i] = strtok(input, " ");                             // Split command
        while (argv[i] != NULL)
        {
            i++;
            argv[i] = strtok(NULL, " ");
        }

        execvp(argv[0], argv);                                    // Execute command
        
        perror("execvp");                                         // Print error if fails
        exit(1);
    }
}


                                                                  // Execute built-in commands
void execute_internal_commands(char *input_string)
{
    char buff[50];                                                // Buffer for storing path

    if (strcmp(input_string, "exit") == 0)                        // Exit shell
    {
        exit(0);
    }
    else if (strcmp(input_string, "pwd") == 0)                    // Print working directory
    {
        getcwd(buff, sizeof(buff));
        printf("%s\n", buff);
    }
    if (strncmp(input_string, "cd", 2) == 0)
    {
        char *path = input_string + 2;

        while (*path == ' ')
            path++;

        if (*path == '\0')
       {
            chdir(getenv("HOME"));
       }
       else if (chdir(path) != 0)
       {
        printf("cd: %s: No such file or directory\n", path);
       }
    }  
    else if (strcmp(input_string, "echo $$") == 0)                // Print shell PID
    {
        printf("%d\n", getpid());
    }
    else if (strcmp(input_string, "echo $?") == 0)                // Print last exit status
    {
        if (WIFEXITED(status))
            printf("%d\n", WEXITSTATUS(status));
        else
            printf("%d\n", status);
    }
    else if (strcmp(input_string, "echo $SHELL") == 0)            // Print shell path
    {
        char *shell = getenv("SHELL");

        if (shell != NULL)
            printf("%s\n", shell);
        else
            printf("SHELL not set\n");
    }

    else if (strcmp(input_string, "jobs") == 0)
        print_jobs();

    else if (strcmp(input_string, "fg") == 0)
    {
        if (head == NULL)
            printf("No jobs\n");
        else
        {
            kill(head->pid, SIGCONT);
            waitpid(head->pid, &status, WUNTRACED);
            delete_first();
        }
    }
    else if (strcmp(input_string, "bg") == 0)
    {
        if (head == NULL)
            printf("No jobs\n");
        else
        {
            kill(head->pid, SIGCONT);
            delete_first();
        }
    }
    else if (strncmp(input_string, "echo ", 5) == 0)              // Echo text
    {
        printf("%s\n", input_string + 5);
    }
    else
    {
        printf("Command not found\n");                            // Invalid command
    }
}

void insert_at_first(pid_t pid_value, char *cmd)   // Function to insert a new job node at the beginning of the linked list
{
    NODE *new = malloc(sizeof(NODE));              // Dynamically allocate memory for a new NODE structure

    new->pid = pid_value;                          // Store the process ID of the stopped/background process

    strcpy(new->cmd, cmd);                         // Copy the command string into the node's cmd field

    new->link = head;                              // Make new node point to current head (link new node to existing list)

    head = new;                                    // Update head pointer to new node (new node becomes first node)
}



void print_jobs(void)                              // Function to display all stopped/background jobs
{
    NODE *temp = head;                             // Create a temporary pointer and initialize it to head node

    while (temp)                                   // Traverse the linked list until temp becomes NULL
    {
        printf("PID: %d  CMD: %s\n", temp->pid, temp->cmd); // Print process ID and corresponding command

        temp = temp->link;                         // Move temp pointer to next node in the list
    }
}



void delete_first(void)                            // Function to delete the first node from the linked list
{
    if (head)                                      // Check if list is not empty
    {
        NODE *temp = head;                         // Store current head node in temporary pointer

        head = head->link;                         // Move head pointer to next node (second node becomes new head)

        free(temp);                                // Free memory of old head node to avoid memory leak
    }
}