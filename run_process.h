#ifndef RUN_PROCESS_H
#define RUN_PROCESS_H

#include <sys/types.h>

int run_process(const char *process, char *const argv[], pid_t &childpid);

#endif // RUN_PROCESS_H

