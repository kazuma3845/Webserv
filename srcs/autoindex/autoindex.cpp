#include "autoindex.hpp"

std::string AutoIndex::creat(std::string uri)
{
	std::string page;
	const char *path = uri.c_str();
	DIR *dir = opendir(path);
	page = "Autoindex";
	return page;
}
