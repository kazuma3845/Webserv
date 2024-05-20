#include "../include/webserv.hpp"
#include <poll.h>

int main(int argc, char **argv)
{
	Web web;

	if (argc != 2)
	{
		std::cout << "Error, Wrong number of arguments" << std::endl;
		return 1;
	}
	try
	{
		if (!web.check(argv[1]))
			throw std::bad_exception();
		web.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	
	return 0;
}
