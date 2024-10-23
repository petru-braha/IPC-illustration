#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <errno.h>

int main()
{
    char server_username[64];
    char buffer[64] = "welcome, petru!";
    if(strstr(buffer, "welcome"))
        strcpy(server_username, buffer + 9);
    server_username[strlen(server_username) - 1] = 0;

    printf("%s\n", server_username);
    return 0;    
}
