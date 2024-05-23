#pragma once

# include <iostream>
# include <string>
# include <algorithm>
# include <fstream>
# include <sstream>


# include <vector>
# include <map>

// using namespace std;

class Web
{
	private:
		std::map<std::string, std::string> _info;

	public:
		Web(){};
		~Web(){};

		void check(char *argv);
		void run() const;
		void clean() const;
};
