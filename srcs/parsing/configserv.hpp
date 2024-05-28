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
		std::map<std::string, std::map<std::string, std::string> > _info;
		std::vector<std::string> _key;

	public:
		configserv(){};
		~configserv(){};

		//additional fonction
		void add_map(std::string line, std::string first_key, int *i);
		void serv(std::vector<std::string> file, unsigned int &i);

};
