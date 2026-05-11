MiniShell – Custom Linux Command Line Interpreter

MiniShell is a Unix/Linux command-line interpreter developed in C that simulates the basic functionality of a Linux terminal. The project was designed to understand how shells work internally by implementing process creation, command execution, and signal handling using Linux system calls.

The shell accepts user commands, parses them, creates child processes, and executes commands using POSIX APIs. It also supports built-in shell commands and handles invalid inputs gracefully.

This project helped in gaining practical exposure to Embedded Linux concepts, system-level programming, and process management.

✨ Features
✅ Execute Linux system commands
✅ Built-in commands support:

cd
pwd
echo
exit

✅ Process creation using fork()
✅ Command execution using execvp()
✅ Parent-child synchronization using wait()
✅ Signal handling (Ctrl+C, Ctrl+Z)
✅ Command parsing and tokenization
✅ Interactive shell prompt
✅ Error handling for invalid commands

⚙️ Working Principle

MiniShell continuously waits for user input through a shell prompt.

Execution Flow:
Read command input from the user
Parse and tokenize the command string
Create a child process using fork()
Execute the command using execvp()
Parent process waits for child completion using wait()
Display the output and return to the prompt
Built-in Commands:

Commands like cd, pwd, echo, and exit are handled internally without creating a separate process.

🧠 Core Concepts Used
Linux System Calls
Process Management
Shell Internals
Parent & Child Processes
fork(), execvp(), wait()
Signal Handling
Command Parsing
File Descriptors
System-Level Programming

🛠️ Technologies Used
Language: C
Platform: Linux / Ubuntu
Compiler: GCC
APIs: POSIX System Calls
Build Tool: Makefile

🎯 Key Learnings
Learned how Linux shells execute commands internally
Gained hands-on experience with process creation and synchronization
Improved understanding of Linux system calls and signal handling
Strengthened debugging and modular programming skills
Developed practical knowledge in Embedded Linux and system programming

🚀 How to Run
gcc *.c -o minishell
./minishell
