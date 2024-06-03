#include "location.hpp"

location::location()
{
	_autoindex = false;
	_client_size = 0;
}

void location::init(std::vector< std::vector<std::string> > file, unsigned int &i)
{
	std::string tab[9] = {"root", "autoindex", "client_max_body_size", "index", "alias", "return", "allow_methods", "cgi_path", "cgi_ext"};
	_name = file[i][1];
	i++;
	for (; file[i][0].compare("}"); i++)
	{
		int j = 0;
		for (; tab[j].compare(file[i][0]) && j < 9; j++)
			;
		switch(j)
		{
			case 0:
			{
				_root = file[i][1];
				break ;
			}
			case 1:
			{
				if (file[i][1].compare("on") == 0)
					_autoindex = true;
				break ;
			}
			case 2:
			{
				_client_size = stoi(file[i][1]); //----------------C++11---------------------------------------------------
				break ;
			}
			case 3:
			{
				_index = file[i][1];
				break ;
			}
			case 4:
			{
				_alias = file[i][1];
				break ;
			}
			case 5:
			{
				_return = file[i][1];
				break ;
			}
			case 6:
			{
				for (unsigned int k = 1; k < file[i].size(); k++)
					_allow_methods.push_back(file[i][k]);
				break ;
			}
			case 7:
			{
				for (unsigned int k = 1; k < file[i].size(); k++)
					_cgi_path.push_back(file[i][k]);
				break ;
			}
			case 8:
			{
				for (unsigned int k = 1; k < file[i].size(); k++)
					_cgi_ext.push_back(file[i][k]);
				break ;
			}
			default:
			{
				throw std::exception();
				break ;
			}
		}
	}
}

void location::print()
{	
	std::cout << "---------------------------" << std::endl;
	std::cout << "Location: " << std::endl;
	std::cout << "Name: " << _name << std::endl;
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Autoindex: " << _autoindex << std::endl;
	std::cout << "Client size: " << _client_size << std::endl;
	std::cout << "Index: " << _index << std::endl;
	std::cout << "Alias: " << _alias << std::endl;
	std::cout << "Return: " << _return << std::endl;
	for (unsigned int i = 0; i < _allow_methods.size(); i++)
		std::cout << "Allow_methods: " << _allow_methods[i] << std::endl;
	for (unsigned int i = 0; i < _cgi_path.size(); i++)
		std::cout << "Cgi_path: " << _cgi_path[i] << std::endl;
	for (unsigned int i = 0; i < _cgi_ext.size(); i++)
		std::cout << "Cgi_ext: " << _cgi_ext[i] << std::endl;
}
