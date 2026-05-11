#include "header.h"                     // Include user-defined header file (contains global variables, macros, structures, and function prototypes)

void scan_input(char *prompt, char *input_string)   // Function to continuously display prompt and read user commands
{
    extract_external_commands(external_commands);   // Read external commands from file and store into global external_commands array

    signal(SIGINT, signal_handler);                 // Register signal handler for SIGINT (Ctrl+C key press)

    signal(SIGTSTP, signal_handler);                // Register signal handler for SIGTSTP (Ctrl+Z key press)

    signal(SIGCHLD, signal_handler);                // Register signal handler for SIGCHLD (child process termination)

    while (1)                                       // Infinite loop to keep the shell running continuously
    {
        printf("%s", prompt);                       // Display the current shell prompt (example: minishell$: )
        fflush(stdout);                             // Flush output buffer immediately to ensure prompt is displayed properly

        scanf(" %[^\n]", input_string);             // Read complete user input including spaces until newline character
        getchar();                                  // Consume leftover newline character from input buffer

    
        if (strncmp(input_string, "PS1=", 4) == 0)  // Check if user is trying to change prompt using PS1=
        {
            char *new_prompt = input_string + 4;    // Point to characters after "PS1="

            if (*new_prompt == '\0' || strchr(new_prompt, ' '))  // If prompt is empty OR contains spaces

                printf("Command not found\n");      // Print error message (invalid prompt format)
            else
            {
                strcpy(prompt, new_prompt);         // Copy new prompt string

                strcat(prompt, " ");                // Add space after prompt for better formatting
            }
            continue;                               // Skip remaining code and go to next iteration
        }

        char temp[100];                             // Temporary buffer to preserve original input
        strcpy(temp, input_string);                 // Copy full input_string into temp buffer

        char *cmd = get_command(temp);              // Extract only first word (actual command)

        int type = check_command_type(cmd);         // Determine whether command is BUILTIN, EXTERNAL, or NO_COMMAND

        if (type == BUILTIN)                        // If command is a built-in command
        {
            execute_internal_commands(input_string); // Execute built-in command using full input string
        }
        else if (type == EXTERNAL)                  // If command is an external command (like ls, cat, etc.)
        {
            pid = fork();                           // Create a new child process

            if (pid > 0)                            // Parent process block
            {
                waitpid(pid, &status, WUNTRACED);   // Wait for child to finish or stop (detect Ctrl+Z)

                pid = -1;                           // Reset pid to indicate no foreground process running
            }
            else if (pid == 0)                      // Child process block
            {
                signal(SIGINT, SIG_DFL);            // Restore default Ctrl+C behavior in child

                signal(SIGTSTP, SIG_DFL);           // Restore default Ctrl+Z behavior in child

                execute_external_commands(input_string);  // Execute external command (supports pipes and arguments)
                
                exit(1);                            // Terminate child process after execution
            }
            else                                    // If fork() fails
            {
                perror("fork");                     // Print error message
                exit(1);                    
            }
        }
        else                                        // If command not found in built-in or external list
        {
            printf("Command not found\n");          // Print error message
        }
    }
}