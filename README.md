Hello 
Simple Shell on Standard C :)
## Desctiption
This is a C program to implement a simple shell command. Only using standard C libraries
(stdio, stdlib, string, termios, unistd) all these headers are part of POSIX C headers
https://en.wikipedia.org/wiki/C_POSIX_library. the input user and the output are managed using putchar () and getchar ()

## Remarks 
This prompt has a prototype very similar to the GNU readline library. and can not handle the execution of the command (for now) or any feature not included in this document. This prompt can handle with  
* read and write the characters included in the ASCII table
* store the user unput into a buffer and return the end of the file (EOT)
* exit the programm by ESC or EOT (in linux CTRL+D, in Windows CRTL+Z)
* return the buffer after KeyEnter 
* handle several keyboard  keys (Left, right , Up, Down, END, HOME, Delete, Bachspace)
* show the history of command by using UP and Down Keys.
* execute two command exit and help to show some informations or exit the programm.  
Offline of this list may show some expected behavior (please report it if you find something :)
### Instalation
This folder contains Makefile on linux, execute the make command in your terminal and open the executable file .header
### Spefications 
* Linux ubunto
* gcc
* standard C99

