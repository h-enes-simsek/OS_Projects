# Unix Shell
I've designed a simple Unix Shell while self-studying as described in Operating System Concepts (Silberschatz, Galvin, and Gagne; 2009, 2011, or 2012).

# Textbook Wants
This project consists of designing a C program to serve as a shell interface
that accepts user commands and then executes each command in a separate
process. Your implementation will support input and output redirection, as
well as pipes as a form of IPC between a pair of commands. Completing this
project will involve using the UNIX fork(), exec(), wait(), dup2(), and
pipe() system calls and can be completed on any Linux, UNIX, or macOS.

# Building
```
git clone git@github.com:h-enes-simsek/UnixShell.git
cd UnixShell 
gcc main.c
./a.out
```

# Todo List
- history
- redirect output
- comm with pipe
- (bug) & does not work properly. After I researched other people's solutions, simple shells have this problem.
