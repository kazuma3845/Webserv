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
	if (value.empty() || key.front() == '#')
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
			if (rest.empty())
				break ;
			value = value + " " + rest;
		}
		if (value.back() == ';')
			value.pop_back();
	}
	this->_info[first_key][key] = value;
	std::cout << "Key: " << key << " | Value: " << this->_info[first_key][key] << " | Size: " << this->_info[first_key].size() << std::endl;
}

void Web::check(char *argv)
{
	std::string line;
	std::ifstream file;
	std::string key;
	bool i = true;
	bool finish = false;

	file.open(argv);
	if (!file)
		throw std::bad_exception();
	this->_key.push_back("serveur");
	getline(file, line);
	while (1)
	{
		if (i)
		{
			if (!getline(file, line))
				break ;
			while (line.back() != '{' && finish == false)
			{
				add_map(line, "serveur");
				if (!getline(file, line))
					finish = true;
			}
			i = false;
		}
		else
		{
			std::stringstream ss(line);
			ss >> key;
			ss >> key;
			this->_key.push_back(key);
			std::cout << std::endl << key << std::endl;
			if (!getline(file, line))
				break ;
			while (line.back() != '{' && finish == false)
			{
				add_map(line, key);
				if (!getline(file, line))
					finish = true;
			}
		}
		if (finish)
			break ;
	}
}

void Web::run() const
{
	std::cout << "RUN" << std::endl;
}

void Web::clean() const
{

}
