# bruhSH
WARNING: I made the whole shell in WSL.

To execute the shell, 2 steps:
step 1: type "make" in the terminal
step 2: type "./a.out" in the terminal

Use "quit" to exit the shell

The Shell's name is BruhSH, the name was an after thought, i was in the middle of understanding foreground processes when i named it. I like the name though.

* main is in bruhSH.c
* most headerfiles are in headers.h
* colors.c defines the functions used to color the output
* buildin.c has echo, cd, and pwd as functions
* ls.c has ls as a function
* pinfo.c has pinfo as a function
* fgarg.c has the foreground and background and signal handling function for the rest of the arguements
* executioner.c contains the if else tree for commands as well as the function for the prompt
* fgarg.c as ctrl c, z and fg
* sbeve.c has jobs, and all the ways to kill em

FG is SUPER buggy and probably will not work.
BG is not implemented.
