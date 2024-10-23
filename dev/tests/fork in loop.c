#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <unistd.h>

#define CHILD 0
#define PARNT 1

int socket_child_server[2], 
    pipe_server_child[2];

int iteration()
{
    char buffer[1024];
    pid_t pid = -1;

    pipe(pipe_server_child);
    socketpair(AF_UNIX, SOCK_STREAM, 0, socket_child_server);
    
    pid = fork();
    
    // parent
    if (pid) 
    { 
      	close(socket_child_server[CHILD]);
        close(pipe_server_child[0]);

      	write(pipe_server_child[1], "first", 5);
        read(socket_child_server[1], buffer, 1024);
      	printf("Parent process: %s\n", buffer);
      	
        close(pipe_server_child[1]);
      	close(socket_child_server[PARNT]);
        
        int child_return_value = -1;
        wait(&child_return_value);
    } 
    
    // child
    else 
    { 
        close(pipe_server_child[1]);
      	close(socket_child_server[PARNT]); 

      	read(pipe_server_child[0], buffer, 1024);
        write(socket_child_server[0], "second", 6); 
      	printf("Child process: %s\n", buffer);

        close(pipe_server_child[0]);
      	close(socket_child_server[CHILD]);
        exit(EXIT_SUCCESS);
    }

    return 1;
}

int main(void) 
{
    iteration();
    return 0;
}
