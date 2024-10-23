#include "dev/apps.h"

//------------------------------------------------
// second to run

int main()
{
    // ux
    unsigned int commands_number = 0;
    call(printf("you can type commands.\n\n"));
    
    // open to send
    int fd_send = open(path_fifo_client_server, O_RDWR);
    if(-1 == fd_send)
    {
        call(mknod(path_fifo_client_server, __S_IFIFO | 0777, 0));
        fd_send = open(path_fifo_client_server, O_RDWR);
        call(fd_send);
    }

    // open to receive
    int fd_receive = open(path_fifo_server_client, O_RDWR);
    if(-1 == fd_receive) 
    {
        call(mknod(path_fifo_server_client, __S_IFIFO | 0777, 0));
        fd_receive = open(path_fifo_server_client, O_RDWR);
        call(fd_receive);
    }
    
    //------------------------------------------------
    // loop until quit
    
    size_t line_size = MAX_COMMAND_SIZE / 4;
    for(;;)
    {
        char* line = 0;
        call(getline(&line, &line_size, stdin));
        char* command = convert_line(line);
        call(write(fd_send, command, strlen(command)));
        free(line);

        char number[3];
        call(read(fd_receive, number, 2 * sizeof(char)));
        number[2] = 0;
        int n_server = atoi(number);

        printf("s: %s\n", command);
        
        char output[MAX_OUTPUT_SIZE];
        call(read(fd_receive, output, n_server));
        output[n_server] = 0;
        call(printf("%s\n\n", output));
        
        if(0 == strcmp(command, "04quit")){
            free(command); break;}
        free(command); 
    }

    call(close(fd_send));
    call(close(fd_receive));

    return EXIT_SUCCESS;
}
