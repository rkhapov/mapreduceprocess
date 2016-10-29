#include <iostream>
#include <fstream>

#include "reduce.hpp"

bool merge_files(const char *file1name, const char *file2name,
                 const char *out_file)
{
    std::ifstream file1(file1name, std::ios::in | std::ios::binary);
    std::ifstream file2(file2name, std::ios::in | std::ios::binary);

    std::ofstream out(out_file,
                      std::ios::out | std::ios::trunc | std::ios::binary);


    std::cout << "Merging files \'" << file1name
              << "\' and \'" << file2name << "\' to file \'"
              << out_file << "\'...";
    std::cout.flush();

    if (!file1.is_open() || !file2.is_open() ||
        !out.is_open())
    {
        std::cout << "fail" << std::endl;
        return false;
    }

    int val1, val2;

    if (!file1.read(reinterpret_cast<char*>(&val1), sizeof(int)))
    {
        while (file2.read(reinterpret_cast<char*>(&val2), sizeof(int)))
        {
            out.write(reinterpret_cast<char*>(&val2), sizeof(int));
        }
        std::cout << "ok" << std::endl;
        return true;
    }

    if (!file2.read(reinterpret_cast<char*>(&val2), sizeof(int)))
    {
        out.write(reinterpret_cast<char*>(&val1), sizeof(int));


        while (file1.read(reinterpret_cast<char*>(&val1), sizeof(int)))
        {
            out.write(reinterpret_cast<char*>(&val1), sizeof(int));
        }

        std::cout << "ok" << std::endl;
        return true;
    }

    while (true)
    {
        if (val1 < val2)
        {
            out.write(reinterpret_cast<char*>(&val1), sizeof(int));

            if (!file1.read(reinterpret_cast<char*>(&val1), sizeof(int)))
            {
                out.write(reinterpret_cast<char*>(&val2), sizeof(int));
                break;
            }
        }
        else
        {
            out.write(reinterpret_cast<char*>(&val2), sizeof(int));

            if (!file2.read(reinterpret_cast<char*>(&val2), sizeof(int)))
            {
                out.write(reinterpret_cast<char*>(&val1), sizeof(int));
                break;
            }
        }
    }

    while (file1.read(reinterpret_cast<char*>(&val1), sizeof(int)))
    {
        out.write(reinterpret_cast<char*>(&val1), sizeof(int));
    }


    while (file2.read(reinterpret_cast<char*>(&val2), sizeof(int)))
    {
        out.write(reinterpret_cast<char*>(&val2), sizeof(int));
    }


    std::cout << "ok" << std::endl;

    return true;
}

bool reduce(const std::vector<std::string> &part_files,
            const char *out_file_name)
{

    std::cout << "Reducing files: ";
    for (std::size_t i = 0; i < part_files.size(); i++)
        std::cout << "\'" << part_files[i] << "\' ";
    std::cout << std::endl;
    std::cout << "with temporary file \'temp.data\' "
              << "to file \'" << out_file_name << "\'" << std::endl;

    std::ofstream tempfile("temp.data", std::ios::out | std::ios::trunc);
    if (!tempfile.is_open())
        return false;
    tempfile.close();

    bool is_current_file_temp = true;

    for (std::size_t i = 0; i < part_files.size(); i++)
    {
        if (is_current_file_temp)
        {
            if (!merge_files(part_files[i].c_str(), "temp.data", out_file_name))
                return false;

            is_current_file_temp = false;
        }
        else
        {
            if (!merge_files(part_files[i].c_str(), out_file_name, "temp.data"))
                return false;

            is_current_file_temp = true;
        }
    }

    if (is_current_file_temp == true) //merged in 'temp.data'
    {
        //copy 'temp.data' to the out_file
        std::cout << "Coping \'temp.data\' to the \'" << out_file_name << "\'...";
        std::cout.flush();

        std::ifstream tempfile("temp.data", std::ios::in | std::ios::binary);

        std::ofstream outfile(out_file_name,
                              std::ios::out | std::ios::trunc | std::ios::binary);

        if (!tempfile.is_open() || !outfile.is_open())
        {
            std::cout << "fail" << std::endl;
            return false;
        }

        int block_size = 100 * 1024 * 1024;

        int *block = new int[block_size];

        int read_size;

        while ((read_size = tempfile.readsome(reinterpret_cast<char*>(block), block_size)))
        {
            outfile.write(reinterpret_cast<char*>(block), read_size);
        }

        delete[] block;

        std::cout << "ok" << std::endl;
    }

    return true;
}
