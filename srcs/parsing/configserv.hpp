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
		std::map<std::string, std::string> _info;
		std::map<std::string, std::string> _none;
		std::map<std::string, std::string> _tours;
		std::map<std::string, std::string> _red;
		std::map<std::string, std::string> _cgi;
 
	public:
		configserv(){};
		~configserv(){};

		//additional fonction
		void add_map(std::string line, unsigned int &i);
		void serv(std::vector<std::string> file, unsigned int &i);

};
