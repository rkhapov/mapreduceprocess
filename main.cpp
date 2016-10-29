#include <iostream>

#include <cerrno>

#include <set>

#include <wait.h>
#include <unistd.h>

#include "run_process.h"
#include "partition.h"
#include "file_generate.hpp"
#include "is_file_exist.h"
#include "is_sorted.hpp"
#include "reduce.hpp"

int main()
{
    std::ios::sync_with_stdio(false);

    if (!is_file_exist("bigdata.data"))
    {
        if (!generate_file("bigdata.data",
                      1U * 1024 * 1024 * 1024, //1 GB
                      200 * 1024 * 1024, //200 MB
                      -5000, 5000))
        {
            std::cout << "Cant generate \'bigdata.data'" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cout << "No generating \'bigdata.data\': already exist"
                  << std::endl;
    }


    /*
     * MAP
     *
     * Partition file
     * and run proccess which will sort this files
    */

    std::vector<std::string> partfiles = partition("bigdata.data",
                                                   500 * 1024 * 1024); //100 MB

    std::set<pid_t> process;

    std::cout << "Running proccess..." << std::endl;

    for (int i = 0; i < (int)partfiles.size(); i++)
    {
        char * const argv[] = {
            (char*)"sortfile",
            (char*)(&partfiles[i][0]),
            nullptr
        };

        pid_t pid;

        if (run_process("sortfile", argv, pid))
        {
            std::cout << "Cant run process sortfile" << std::endl;
            return 1;
        }

        std::cout << "Process " << pid << " runned with file \'"
                  << partfiles[i] << "\'" << std::endl;

        process.insert(pid);
    }

    std::cout << "Waiting for all process" << std::endl;

    while (!process.empty())
    {
        int status;

        pid_t pid = wait(&status);

        if (pid == -1 && errno == ECHILD)
        {
            std::cout << "Strange error: no child process" << std::endl;
            return 1;
        }

        if (status)
        {
            std::cout << pid << " returned non-zero exit code" << std::endl;
            return 1;
        }

        std::cout << "process " << pid << " finished" << std::endl;
        process.erase(pid);
    }

    for (int i = 0; i < (int)partfiles.size(); i++)
    {
        is_file_sorted(partfiles[i].c_str());
    }


    /*
     * REDUCE
     * merging files
    */

    reduce(partfiles, "bigdatasorted.data");

    is_file_sorted("bigdatasorted.data");

    return 0;
}
