#include <iostream>

#include <unistd.h>

#include "run_process.h"

int run_process(const char *process, char * const argv[], pid_t &childpid)
{
    childpid = fork();

    if (childpid < 0)
        return 1;
    else if (childpid == 0)
    {
        execv(process, argv);
        return 2;
    }

    return 0;
}
