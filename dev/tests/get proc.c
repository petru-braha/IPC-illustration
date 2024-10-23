#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>

#include <string.h>
#include <errno.h>

#include "dev/apps.h"

char server_username[MAX_STRING_SIZE] = "petru";

bool is_client_logged()
{
    if(0 == server_username[0])
        return false;
    return true;
}

char* get_argument(const char* const command)
{
    return strstr(command, " : ") + 3;
}

char* get_proc_info(const char* const command)
{
    // case 0
    if(!is_client_logged())
    {
        const char buffer[] = "you must login before.";
        return convert_line(buffer);
    }

    char pid[MAX_STRING_SIZE];
    strcpy(pid, get_argument(command));

    char path[MAX_STRING_SIZE] = "/proc/";
    strcat(path, pid);
    strcat(path, "/status");

    FILE* file = fopen(path, "r");
    
    // invalid pid
    if(NULL == file)
    {
        const char buffer[] = "invalid pid.";
        return convert_line(buffer);
    }
    
    // valid pid
    char buffer[MAX_OUTPUT_SIZE];
    size_t line_size = MAX_OUTPUT_SIZE; 
        
    for(;;)
    {
        char* line = 0;
        int bytes = getline(&line, &line_size, file);
        var_call(bytes);
        if(0 == bytes)
            break;

        if(strstr(line, "Name: ") ||
            strstr(line, "State: ") ||
            strstr(line, "PPid: ") ||
            strstr(line, "Uid: "))
            strcat(buffer, line);
        if(strstr(line, "VmSize: "))
        {
            strcat(buffer, line);
            break;
        }

        free(line);
    }

    return convert_line(buffer);
}

int main()
{
    char* output = get_proc_info("get-proc-info : 159");
    printf("%s\n", output);
    free(output); 
    return 0;
}