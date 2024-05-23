#include "../include/webserv.hpp"

int main(int argc, char **argv)
{
	Web web;

	if (argc != 2)
	{
		std::cerr << "Error, Wrong number of arguments" << std::endl;
		return 1;
	}
	try
	{
		web.check(argv[1]);	// init struct and check error
		// web.run();		// Run programme
		// web.clean();		// Clean all
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}

void Web::check(char *argv)
{
	std::string line;
	std::string key;
	std::string value;
	std::ifstream file;

	file.open(argv);
	if (!file)
		throw std::bad_exception();
	getline(file, line);
	
	std::cout << line << std::endl;
	while (getline(file, line) && !line.empty())
	{
    	std::stringstream ss(line);
    	ss >> key >> value;
		value.pop_back();
		this->_info[key] = value;
    	// std::cout << this->_info[key] << std::endl;
	}
}

void Web::run() const
{
	std::cout << "RUN" << std::endl;
}

void Web::clean() const
{

}
