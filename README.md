# IPC illustration

## description

- inter-process communication made with:
    - pipes
    - fifos
    - sockets
- this program runs up to three processes:
    - client process
    - server process
    - server's child process 
- C programming language was used

## setup

- run server.c
- run client.c
- write commands in the client process

## signatures of commands
 
- login : argument
- lougout
- get-logged-users
- get-proc-info : argument
- quit

## projects' design

- client sends the command to server
- the server receives it and sends the command to a child process 
- check the requirements 
- send the output of the command back to the client

## valuable points

- fast execution and portability assured with POSIX primitives
- failures are avoided by the constructed error management system

## limitations

- small constants
- support for modest amount of data

## upcoming improvements

- greater constants and more data storage power
- the patch for server.c's username
- better way of shutting down the server
- get-proc-info debug
