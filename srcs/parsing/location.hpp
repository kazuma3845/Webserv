#pragma once

# include <iostream>
# include <string>

# include <map>
# include <vector>

class location
{
	private:
		std::string _name;
		std::string _root;
		bool _autoindex;
		unsigned int _client_size;
		std::string _index;
		std::string _alias;
		std::string _return;
		std::vector<std::string> _allow_methods;
		std::vector<std::string>	_cgi_path;
		std::vector<std::string>	_cgi_ext;
 
	public:
		location();
		~location(){};

		void init(std::vector< std::vector<std::string> > file, unsigned int &i);
		void print();
};
