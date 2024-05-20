#include "../include/webserv.hpp"

Web::Web()
{

}

Web::~Web()
{

}

bool Web::check(char *argv) const
{
	if (argv[0] == 'e')
		return false;
	return true;
}

void Web::start() const
{
	std::cout << "TEST" << std::endl;
}
