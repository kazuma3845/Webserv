#include "../include/webserv.hpp"

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
