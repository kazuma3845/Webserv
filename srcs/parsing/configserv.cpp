# include "configserv.hpp"

// void configserv::add_map(std::string line, unsigned int &i)
// {
// 	std::string key;
// 	std::string value;
// 	std::string rest;
// 	std::stringstream ss(line);

// 	ss >> key >> value;
// 	if (value.empty() || key.front() == '#')
// 		return ;
// 	if (value.back() == '}')
// 		value.pop_back();
// 	if (value.back() == ';')
// 		value.pop_back();
// 	else
// 	{
// 		while (value.back() != ';')
// 		{
// 			ss >> rest;
// 			if (rest.empty())
// 				break ;
// 			value = value + " " + rest;
// 		}
// 		if (value.back() == ';')
// 			value.pop_back();
// 	}
// 	if (this->_info.find(key) != this->_info.end())
// 		key = key + std::to_string(++i);
// 	this->_info[key] = value;
// 	std::cout << "Key: " << key << " | Value: " << this->_info[key] << " | Size: " << this->_info.size() << std::endl;
// }


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
			_location.push_back(a);
		}
		else
		{
			int j = 0;
			for (; tab[j].compare(file[i][0]); j++)
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
					_name.push_back(file[i][1]);
					break ;
				}
				case 2:
				{
					_host.push_back(file[i][1]);
					break ;
				}
				case 3:
				{
					_root.push_back(file[i][1]);
					break ;
				}
				case 4:
				{
					_autoindex.push_back(file[i][1]);
					break ;
				}
				case 5:
				{
					_client_size.push_back(file[i][1]);
					break ;
				}
				case 6:
				{
					_html.push_back(file[i][1]);
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
	for (unsigned int i = 0; i < _name.size(); i++)
		std::cout << "Name: " << _name[i] << std::endl;
	for (unsigned int i = 0; i < _host.size(); i++)
		std::cout << "Host: " << _host[i] << std::endl;
	for (unsigned int i = 0; i < _root.size(); i++)
		std::cout << "Root: " << _root[i] << std::endl;
	for (unsigned int i = 0; i < _autoindex.size(); i++)
		std::cout << "Autoindex: " << _autoindex[i] << std::endl;
	for (unsigned int i = 0; i < _client_size.size(); i++)
		std::cout << "Client size: " << _client_size[i] << std::endl;
	for (unsigned int i = 0; i < _html.size(); i++)
		std::cout << "HTML: " << _html[i] << std::endl;
	std::cout << "Error num: " << _error << " | Path: " << _errorpath[_error] << std::endl;
}