#ifndef PARTITION_H
#define PARTITION_H

#include <cstddef>
#include <string>
#include <vector>

std::vector<std::string> partition(const char *filename,
                                   std::size_t part_size);

#endif // PARTITION_H

