#include "../include/webserv.hpp"

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
		web.check(argv[1]); // init struct and check error
		web.run();	// Run programme
		web.clean();	// Clean all
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}

Web::Web()
{

}

Web::~Web()
{

}

void Web::check(char *argv) const
{
	if (argv[0] == 'e')
		throw std::bad_exception();
}

void Web::run() const
{
	std::cout << "TEST" << std::endl;
}

void Web::clean() const
{

}
