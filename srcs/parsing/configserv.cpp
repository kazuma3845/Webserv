# include "configserv.hpp"

void configserv::serv(std::vector< std::vector<std::string> > file, unsigned int &i)
{
	std::string tab[8] = {"listen", "server_name", "host", "root", "autoindex", "client_max_body_size", "index", "error_page"};
	for (; i < file.size(); i++)
	{
		if (file[i][0].compare("}") == 0)
			return ;
		if (file[i][0].compare("#") == 0)
			;
		else if (file[i][0].compare("location") == 0)
		{
			location a;
			a.init(file, i);
			_location.push_back(a);
		}
		else
		{
			int j = 0;
			for (;j < 8 && tab[j].compare(file[i][0]); j++)
				;
			switch(j)
			{
				case 0:
				{
					_listen.push_back(file[i][1]);
					break ;
				}
				case 1:
				{
					_name = file[i][1];
					break ;
				}
				case 2:
				{
					_host = file[i][1];
					break ;
				}
				case 3:
				{
					_root = file[i][1];
					break ;
				}
				case 4:
				{
					if (file[i][1].compare("on") == 0)
						_autoindex = true;
					break ;
				}
				case 5:
				{
					_client_size = stoi(file[i][1]); //----------------C++11---------------------------------------------------
					break ;
				}
				case 6:
				{
					_html = file[i][1];
					break ;
				}
				case 7:
				{
					_error = file[i][1];
					_errorpath[_error] = file[i][2];
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
}


void configserv::print()
{
	for (unsigned int i = 0; i < _listen.size(); i++)
		std::cout << "Listen: " << _listen[i] << std::endl;
	std::cout << "Name: " << _name << std::endl;
	std::cout << "Host: " << _host << std::endl;
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Autoindex: " << _autoindex << std::endl;
	std::cout << "Client size: " << _client_size << std::endl;
	std::cout << "HTML: " << _html << std::endl;
	std::cout << "Error num: " << _error << " | Path: " << _errorpath[_error] << std::endl;
	for (unsigned int i = 0; i < _location.size(); i++)
	{
		_location[i].print();
	}
}
