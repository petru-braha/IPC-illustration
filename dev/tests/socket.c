#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define CHILD 0
#define PARNT 1

int socket_child_server[2], 
    pipe_server_child[2];

// server child communication model 
int main(void) 
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

      	read(socket_child_server[1], buffer, 1024);
      	printf("Parent process: %s\n", buffer);
      	write(pipe_server_child[1], "first", 5);

        close(pipe_server_child[1]);
      	close(socket_child_server[PARNT]);
    } 
    
    // child
    else 
    { 
        close(pipe_server_child[1]);
      	close(socket_child_server[PARNT]); 

      	write(socket_child_server[0], "second", 6); 
      	read(pipe_server_child[0], buffer, 1024);
        printf("Child process: %s\n", buffer);

        close(pipe_server_child[0]);
      	close(socket_child_server[CHILD]);
    }
    
    return 0;
}
