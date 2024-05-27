#pragma once

# include <iostream>
# include <string>
# include <algorithm>
# include <fstream>
# include <sstream>

# include <map>
# include <vector>

// using namespace std;

class Web
{
	private:
		std::map<std::string, std::map<std::string, std::string> > _info;
		std::vector<std::string> _key;

	public:
		Web(){};
		~Web(){};

		//main fonction
		void check(char *argv);
		void run() const;
		void clean() const;

		//additional fonction
		void add_map(std::string line, std::string first_key);
};
