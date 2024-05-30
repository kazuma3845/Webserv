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
	for (; i < file.size(); i++)
	{
		if (file[i][0].compare("}") == 0)
			return ;
		if (file[i][0].compare("#") == 0)
			;
		else if (file[i][0].compare("location") == 0)
		{
			i++;
			for (unsigned int j = 0; file[i][0].compare("}"); i++)
				std::cout << file[i][j] << std::endl;
		}
		// else if ()
		// {

		// }
		else
		{
			std::cout << file[i][0] << std::endl;
		}
		std::cout << "-----------------" << std::endl;
	}
}
