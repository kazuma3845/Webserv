#pragma once

# include <iostream>
# include <string>
# include <algorithm>
# include <fstream>
# include <sstream>

# include "../srcs/parsing/configserv.hpp"

# include <map>
# include <vector>

// using namespace std;

class Web
{
	private:
		std::vector<configserv> _serv;

	public:
		Web(){};
		~Web(){};

		//main fonction
		void parsing(char *argv);
		void run() const;
		void clean() const;

};
