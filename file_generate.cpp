#include <iostream>
#include <fstream>
#include <cstdlib>

#include "file_generate.hpp"

int randint(int min, int max)
{
    return max + std::rand() % (max - min + 1);
}

bool generate_file(const char *filename,
                   std::size_t file_size,
                   std::size_t block_size,
                   int minval, int maxval)
{
    std::ofstream file(filename,
                       std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file)
        return false;

    std::size_t current_size, last_percent;
    current_size = last_percent = 0;

    if (block_size % sizeof(int) || block_size == 0)
    {
        block_size += sizeof(int) - block_size % sizeof(int);
    }

    std::size_t values_per_block = block_size / sizeof(int);

    int *block = new int[values_per_block];

    std::cout << "generating \'" << filename << "\', size = " << file_size << std::endl;

    while (current_size < file_size)
    {
        for (int i = 0; i < (int)values_per_block; i++)
        {
            block[i] = randint(minval, maxval);
        }

        file.write(reinterpret_cast<char*>(block), block_size);

        current_size += block_size;

        std::size_t current_percent = (static_cast<double>(current_size) / file_size) * 100;

        if (current_percent != last_percent)
        {
            std::cout << current_percent << "%";
            std::cout.flush();

            last_percent = current_percent;
        }
    }

    std::cout << std::endl;
    file.close();

    delete[] block;

    return true;
}
