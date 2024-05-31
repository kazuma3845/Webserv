#pragma once

# include <iostream>
# include <string>
# include <algorithm>
# include <fstream>
# include <sstream>

# include <map>
# include <vector>

// using namespace std;

class configserv
{
	private:
		std::vector<std::string> _listen;
		std::vector<std::string> _name;
		std::vector<std::string> _host;
		std::vector<std::string> _root;
		std::vector<std::string> _autoindex;
		std::vector<std::string> _client_size;
		std::vector<std::string> _html;
		std::map<std::string, std::string> _error;
		std::map<std::string, std::string> _rest;

		//location
		std::map<std::string, std::string> _none;
		std::map<std::string, std::string> _tours;
		std::map<std::string, std::string> _red;
		std::map<std::string, std::string> _cgi;
 
	public:
		configserv(){};
		~configserv(){};

		//additional fonction
		void serv(std::vector< std::vector<std::string> > file, unsigned int &i);
		void print() const;

};
