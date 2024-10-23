#ifndef _0COMMAND0_
#define _0COMMAND0_

#include <stdbool.h>
#include <utmp.h>
#include "apps.h"

char server_username[MAX_STRING_SIZE] = "";

//------------------------------------------------
// help:

bool is_client_logged()
{
    if(0 == server_username[0])
        return false;
    return true;
}

bool is_present(const char* const username) 
{
    FILE* file = fopen(path_database, "r");
    char* line = 0;
    size_t line_size = 0;

    while(getline(&line, &line_size, file) != -1)
    {
        line[strlen(line) - 1] = '\0';
        if(0 == strcmp(line, username))
            return true;
    }
    
    return false;
}

char* get_argument(const char* const command)
{
    return strstr(command, " : ") + 3;
}

//------------------------------------------------
// api:

char* get_logged_users()
{
    if(!is_client_logged())
    {
        const char buffer[] = "you must login before.";
        return convert_line(buffer);
    }
    
    setutent();
    char buffer[MAX_OUTPUT_SIZE];
    for(struct utmp *user = getutent(); user; user = getutent()) 
    {
        // compute partial buffer
        char user_info[MAX_STRING_SIZE];
        if (USER_PROCESS == user->ut_type) 
        {
            strcat(buffer, "user: \""); // maximum 32 bytes
            strcat(buffer, user->ut_user);
            //strcat(buffer, "\", host: \""); // maximum 256 bytes
            //strcat(buffer, user->ut_host);
            strcat(buffer, "\", time: ");
            
            char number[MAX_STRING_SIZE];
            sprintf(number, "%d", user->ut_tv.tv_sec);
            strcat(buffer, number);
            strcat(buffer, "\n");
        }

        // add to final buffer
        strcat(buffer, user_info);
    }
    
    endutent();
    return convert_line(buffer);
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

//------------------------------------------------

char* const login_user(const char* const command)
{    
    char* username = get_argument(command);
    if(is_present(username) && false == is_client_logged())
    {
        strcpy(server_username, username);

        char buffer[MAX_OUTPUT_SIZE] = "welcome, ";
        strcat(buffer, username);
        strcat(buffer, "!");
        return convert_line(buffer);
    }
    
    const char buffer[] = "can not conenct.";
    return convert_line(buffer);
}

char* logout_user()
{
    if(is_client_logged())
    {
        char buffer[MAX_OUTPUT_SIZE] = "we will miss you, ";
        strcat(buffer, server_username);
        strcat(buffer, "...");

        size_t n = strlen(server_username);
        for(size_t i = 0; i < n; i++)
            server_username[i] = 0;

        return convert_line(buffer);
    }

    const char buffer[] = "you're not logged-in.";
    return convert_line(buffer);
}

char* quit()
{
    const char buffer[] = "closing appilication.";
    return convert_line(buffer);
}

//------------------------------------------------
// command parser:

char* parse_command(const char* const command)
{
    if('q' == command[0])
        if(0 == strcmp(command, "quit"))
            return quit();

    // logs
    if('l' == command[0])
    {
        if(0 == strcmp(command, "logout"))
            return logout_user();
        if(NULL != strstr(command, "login : ") &&
        strlen(command) > 9)
            return login_user(command);
    }

    // queries
    if('g' == command[0])
    {
        if(0 == strcmp(command, "get-logged-users"))
            return get_logged_users();
        if(NULL != strstr(command, "get-proc-info : ") &&
            strlen(command) > 17)
            return get_proc_info(command);
    }

    // invalid
    const char buffer[] = "invalid command.";
    return convert_line(buffer);
}

#endif
