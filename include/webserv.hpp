#pragma once

# include <iostream>
# include <string>
# include <algorithm>
# include <fstream>
# include <sstream>

# include "../srcs/parsing/configserv.hpp"
# include "../srcs/server/Server.hpp"
# include <map>
# include <vector>

// using namespace std;

class Server;

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

		//exception
		class ExceptionErrorFile : public std::exception
		{
			public:
				const char *what() const throw();
		};

		class ExceptionInFile : public std::exception
		{
			public:
				const char *what() const throw();
		};

};
