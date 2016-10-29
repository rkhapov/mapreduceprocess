#include <iostream>
#include <fstream>

#include "partition.h"

std::vector<std::string> partition(const char *filename,
                                   std::size_t part_size)

{
    std::cout << "Partition of file \'" << filename << "\'..." << std::endl;

    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file.is_open())
    {
        return std::vector<std::string>();
    }

    std::vector<std::string> files;
    int file_num = 0;

    if (part_size % sizeof(int))
    {
        part_size += sizeof(int) - part_size % sizeof(int);
    }

    std::vector<int> block(part_size / sizeof(int));

    int read_len;

    while ((read_len = file.readsome(reinterpret_cast<char*>(&block[0]),
                                     part_size)))
    {
        std::ofstream part("part" + std::to_string(file_num) + ".data",
                           std::ios::out | std::ios::trunc | std::ios::binary);

        if (!part.is_open())
        {
            std::cout << "Cant create part file #" << file_num << std::endl;
            return std::vector<std::string>();
        }

        part.write(reinterpret_cast<char*>(&block[0]), read_len);

        part.close();

        std::cout << "Created file # " << file_num << std::endl;

        files.push_back("part" + std::to_string(file_num) + ".data");

        file_num++;
    }

    return files;
}
