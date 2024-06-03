#pragma once

# include <iostream>
# include <string>
# include <algorithm>
# include <fstream>
# include <sstream>

# include <map>
# include <vector>

class location
{
	private:
		std::vector<std::string> _listen;
		std::vector<std::string> _name;
		std::vector<std::string> _host;
		std::vector<std::string> _root;
		std::vector<std::string> _autoindex;
		std::vector<std::string> _client_size;
		std::vector<std::string> _html;
		std::string _error;
		std::map<std::string, std::string> _errorpath;
 
	public:
		location(){};
		~location(){};

};
