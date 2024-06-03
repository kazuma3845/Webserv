#pragma once

class configserv;

# include <iostream>
# include <string>
# include <algorithm>
# include <fstream>
# include <sstream>

# include <map>
# include <vector>

# include "location.hpp"

// using namespace std;

class configserv
{
	private:
		std::vector<std::string> _listen;
		std::string _name;
		std::string _host;
		std::string _root;
		bool _autoindex;
		unsigned int _client_size;
		std::string _html;
		std::string _error;
		std::map<std::string, std::string> _errorpath;
		std::vector<location> _location;

 
	public:
		configserv(){};
		~configserv(){};

		//additional fonction
		void serv(std::vector< std::vector<std::string> > file, unsigned int &i);
		void print();

};
