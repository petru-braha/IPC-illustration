# IPC illustration

## description

- portability assured with POSIX calls
- C programming language
- error checking for every primitive

## setup

- run server.c
- run client.c
- write commands in the client process

## defined commands

- login : argument
- lougout
- get-logged-users
- get-proc-info : argument
- quit

## projects' design:

- client sends the command to server
- the server receives it and sends the command to a child process 
- check the requirements 
- send the output of the command back to the client
