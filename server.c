#include <sys/socket.h>
#include <utmp.h>
#include "dev/command.h"

#define CHILD 0
#define PARNT 1

//------------------------------------------------
// first to run:

char* iteration(const char* const command, const int command_size)
{
    int pipe_server_child[2]; 
    int socket_child_server[2];
    
    pipe(pipe_server_child);
    socketpair(AF_UNIX, SOCK_STREAM, 0, socket_child_server);

    char* output = 0;

    // parent
    pid_t pid = fork();
    if (pid) 
    { 
      	close(socket_child_server[CHILD]);
        close(pipe_server_child[READ]);
        
        write(pipe_server_child[WRIT], command, command_size);
        
        char number[3];
      	read(socket_child_server[1], number, 2);
        number[2] = 0;
        int output_size = atoi(number);
        char buffer[MAX_OUTPUT_SIZE];
      	read(socket_child_server[1], buffer, output_size);
        buffer[output_size] = 0;
        output = (char*)malloc(2 + output_size);
        strcpy(output, number);
        strcat(output, buffer);
        
        // patch
        if(strstr(buffer, "welcome"))
            strcpy(server_username, buffer + 9);
        if('!' != buffer[strlen(buffer) - 1])
          server_username[strlen(server_username) - 1] = 0;
        if(strstr(buffer, "we will miss"))
            for(size_t i = 0; i < MAX_STRING_SIZE; i++)
                server_username[i] = 0;

        close(pipe_server_child[WRIT]);
      	close(socket_child_server[PARNT]);
        
        int child_return_value = -1;
        wait(&child_return_value);
    } 
    
    // child
    else 
    { 
        close(pipe_server_child[WRIT]);
      	close(socket_child_server[PARNT]); 

        char buffer[MAX_COMMAND_SIZE];
        read(pipe_server_child[0], buffer, command_size);
        buffer[command_size] = 0;
        
        char* message = parse_command(buffer);
      	write(socket_child_server[0], message, strlen(message)); 
      	free(message);

        close(pipe_server_child[READ]);
      	close(socket_child_server[CHILD]);
        exit(EXIT_SUCCESS);
    }

    return output;
}

int main() // fork inside the loop
{
    printf("the server is online.\n\n");
    
    int fd_command = open(path_fifo_client_server, O_RDWR);
    int fd_output = open(path_fifo_server_client, O_RDWR);
    call(fd_command);
    call(fd_output);
    
    for(;;)
    {
        // read
        char number[3];
        call(read(fd_command, number, 2 * sizeof(char)));
        number[2] = 0;
        int n_server = atoi(number);
        
        char command[MAX_COMMAND_SIZE];
        call(read(fd_command, command, n_server));
        command[n_server] = 0;

        // write
        char* output = iteration(command, n_server);
        call(write(fd_output, output, strlen(output)));
        
        printf("%s\n%s\n\n", output, server_username);

        if(strstr(output, "closing ")){
            free(output); break; }
        free(output);
    }

    call(close(fd_command));
    call(close(fd_output));
    return 0;
}
