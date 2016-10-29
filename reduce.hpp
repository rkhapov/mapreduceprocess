#ifndef REDUCE_HPP
#define REDUCE_HPP

//giving files part0.data...partN.data
//and creating one file which containes sorted data
//of part-files

#include <vector>
#include <string>

bool reduce(const std::vector<std::string> &part_files,
            const char *out_file_name);

#endif // REDUCE_HPP

