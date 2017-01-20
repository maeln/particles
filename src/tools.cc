#include <fstream>
#include <sstream>

#include "tools.hh"

std::string read_file(std::string path)
{
	std::ifstream f(path);
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}
