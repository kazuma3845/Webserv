# include "configserv.hpp"

void configserv::add_map(std::string line, unsigned int &i)
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
	if (this->_info.find(key) != this->_info.end())
		key = key + std::to_string(++i);
	this->_info[key] = value;
	std::cout << "Key: " << key << " | Value: " << this->_info[key] << " | Size: " << this->_info.size() << std::endl;
}


void configserv::serv(std::vector<std::string> file, unsigned int &i)
{
	if (file[i].compare("location") == 0)
	{

	}
	else
	{

	}
}
