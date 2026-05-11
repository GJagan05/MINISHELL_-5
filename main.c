#include "header.h"                     // Include user-defined header file (contains declarations, macros, prototypes)

pid_t pid = -1;                         // Global variable to store process ID of child process (initialized to -1 meaning no process running)

int status = 0;                         // Global variable to store exit status of child process

char input_string[100];                 // Global character array to store user input command (max 100 characters)

char *external_commands[200];           // Array of character pointers to store list of external commands (max 200 commands)

int main()                              // Main function – program execution starts here
{
    system("clear");                    // Clears the terminal screen using system call

    char prompt[25] = "minishell$: ";   // Declare and initialize shell prompt string

    scan_input(prompt, input_string);   // Call function to display prompt and read user input continuously

    return 0;                           // Return 0 to indicate successful execution of program
}
