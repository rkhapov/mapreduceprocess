#ifndef FILE_GENERATOR_HPP
#define FILE_GENERATOR_HPP

#include <cstddef>

bool generate_file(const char *filename,
                   std::size_t file_size,
                   std::size_t block_size,
                   int minval, int maxval);


#endif // FILE_GENERATOR_HPP

