#include "dev/apps.h"
#include "dev/command.h"

int fd_send, fd_receive;
//"../fifo/client-server"
//"../fifo/server-client"

int main()
{
    printf("the server is online.\n\n");

    fd_send = open(path_fifo_client_server, O_RDWR);
    fd_receive = open(path_fifo_server_client, O_RDWR);
    call(fd_send);
    call(fd_receive);
    
    for(;;)
    {
        // read
        char number[3];
        call(read(fd_send, number, 2 * sizeof(char)));
        number[2] = 0;
        int n_server = atoi(number);
        
        char command[MAX_COMMAND_SIZE];
        call(read(fd_send, command, n_server));
        command[n_server] = 0;

        // write
        char* output = parse_command(command);
        call(write(fd_receive, output, strlen(output)));
        
        if(0 == strcmp(command, "quit")){
            free(output); break;}
        free(output);
    }

    call(close(fd_send));
    call(close(fd_receive));
    return 0;
}