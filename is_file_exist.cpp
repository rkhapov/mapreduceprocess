#include <iostream>
#include <fstream>

#include "is_file_exist.h"

bool is_file_exist(const char *path)
{
    std::ifstream file(path);

    if (file.is_open())
    {
        file.close();
        return true;
    }

    return false;
}
