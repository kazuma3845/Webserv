#include "../include/webserv.hpp"

int main(int argc, char **argv)
{
	Web web;

	if (argc < 2)
	{
		std::cerr << "Error, Wrong number of arguments" << std::endl;
		return 1;
	}
	try
	{
		web.parsing(argv[1]);	// init struct and check error
		// web.run();		// Run programme
		// web.clean();		// Clean all
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}

void Web::parsing(char *argv)
{
	std::ifstream file;
	std::vector<std::string> fileline;
	std::string line;
	unsigned int i = 0;
	file.open(argv);
	if (!file)
		throw std::bad_exception();
	for (; getline(file, line); i++)
		fileline.push_back(line);
	file.close();
	// for (unsigned int j = 0; j < i; j++)
	// 	std::cout << fileline[j] << std::endl;
	unsigned int filesize = fileline.size();
	i = 0;
	for (; i < filesize; i++)
	{
		// if (fileline[i].compare("server {"))
		// 	throw std::exception();
		// else
		// {
			try
			{
				configserv server;
				server.serv(fileline, ++i);
				this->_serv.push_back(server);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error: " << e.what() << std::endl;
			}
		// }
	}
}

void Web::run() const
{
	std::cout << "RUN" << std::endl;
}

void Web::clean() const
{

}
