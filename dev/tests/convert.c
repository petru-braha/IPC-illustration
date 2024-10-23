#include "../apps.h"
#include "../command.h"

int main()
{
    char* line = convert_line("logout\n");
    printf("%s\n", line);
    free(line);

    line = convert_line("logout\n");
    printf("%s\n", line);
    free(line);

    return 0;
}
