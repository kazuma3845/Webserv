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
	std::vector<std::string> word;
	std::vector< std::vector<std::string> > fileline;
	std::string line;
	unsigned int i = 0;
	file.open(argv);
	if (!file)
		throw std::bad_exception();
	for (; getline(file, line); i++)
	{
		if (!line.empty())
		{
			std::stringstream ss(line);
			for (; ss;)
			{
				std::string w;
				ss >> w;
				if (!w.empty())
				{
					if (w.back() == ';')
						w.pop_back();
					word.push_back(w);
				}
			}
			fileline.push_back(word);
			word.clear();
		}
	}
	file.close();
	unsigned int filesize = fileline.size();
	i = 0;
	for (; i < filesize; i++)
	{
		if (fileline[i][0].compare("server"))
			throw std::exception();
		else
		{
			if (fileline[i][0].compare("{") == 0)
				throw std::exception();
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
		}
	}
	_serv.front().print();
}

void Web::run() const
{
}

void Web::clean() const
{

}
