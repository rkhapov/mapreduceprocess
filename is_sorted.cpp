#include <iostream>
#include <fstream>

#include "is_sorted.hpp"

bool is_sorted_array(const int *arr, int n)
{
    for (int i = 1; i < n; i++)
    {
        if (arr[i] < arr[i - 1])
            return false;
    }

    return true;
}

bool is_file_sorted(const char *filename)
{
    std::cout << "Checking for sorted file \'" << filename << "\'...";
    std::cout.flush();

    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file)
    {
        std::cout << "fail" << std::endl;
        return false;
    }

    int block_size = 2 * 1024 * 1024;

    int *block = new int[block_size / sizeof(int)];

    int read_size = file.readsome(reinterpret_cast<char*>(block), block_size);

    int prev_val = block[read_size / sizeof(int) - 1];

    while (read_size != 0)
    {
        if (prev_val > block[read_size / sizeof(int) - 1] ||
            !is_sorted_array(block, read_size / sizeof(int)))
        {
            std::cout << "fail" << std::endl;
            return false;
        }

        prev_val = block[read_size / sizeof(int) - 1];

        read_size = file.readsome(reinterpret_cast<char*>(block), block_size);
    }

    delete[] block;

    std::cout << "ok" << std::endl;
    return true;
}
