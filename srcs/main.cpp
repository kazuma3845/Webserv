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

void Web::add_map(std::string line, std::string first_key)
{
	std::string key;
	std::string value;
	std::string rest;
	std::stringstream ss(line);

	ss >> key >> value;
	if (value.empty())
		return ;
	if (value.back() == '}')
		value.pop_back();
	if (value.back() == ';')
		value.pop_back();
	else
	{
		while (value.back() != ';')
		{
			ss >> rest;
			value = value + " " + rest;
		}
		value.pop_back();
	}
	this->_info[first_key][key] = value;
	// std::cout << "Key: " << key << " | Value: " << this->_info[first_key][key] << std::endl;
}

void Web::check(char *argv)
{
	std::string line;
	std::ifstream file;
	std::string key;

	file.open(argv);
	if (!file)
		throw std::bad_exception();
	getline(file, line);
	while (getline(file, line) && !line.empty())
		add_map(line, "serveur");
	getline(file, line);
	std::stringstream ss(line);
	ss >> key;
	ss >> key;
	// std::cout << std::endl << key << std::endl;
	while (getline(file, line) && !line.empty())
		add_map(line, key);
	getline(file, line);
	std::stringstream ssi(line);
	ssi >> key;
	ssi >> key;
	// std::cout << std::endl << key << std::endl;
	while (getline(file, line) && !line.empty())
		add_map(line, key);
}

void Web::run() const
{
	std::cout << "RUN" << std::endl;
}

void Web::clean() const
{

}
