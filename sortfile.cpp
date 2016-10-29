#include <iostream>
#include <fstream>

#include <vector>

#include <algorithm>


int main(int argc, char **argv)
{
	std::ios::sync_with_stdio(false);
	
	if (argc < 2)
	{
		return 1;
	}
	
	std::ifstream filein(argv[1], std::ios::in | std::ios::binary);
	
	if (!filein.is_open())
		return 2;
		
	std::vector<int> data;
	
	int block[1024];
	
	int read_size;
	
	while ((read_size = filein.readsome(reinterpret_cast<char*>(block), 1024 * sizeof(int))))
	{
		for (int i = 0; i < read_size / (int)sizeof(int); i++)
		{
			data.push_back(block[i]);
		}
	}
	
	std::sort(data.begin(), data.end());
	
	filein.close();
	
	std::ofstream fileout(argv[1], std::ios::out | std::ios::trunc | std::ios::binary);
	
	if (!fileout.is_open())
		return 2;
		
	fileout.write(reinterpret_cast<char*>(&data[0]), data.size() * sizeof(int));
	
	return 0;
}
