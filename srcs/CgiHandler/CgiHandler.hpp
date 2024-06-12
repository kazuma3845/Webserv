#pragma once

#include <map>
#include <iostream>
#include "../request/request.hpp"

class CgiHandler
{
	private:
		std::string _body;
		std::map<std::string, std::string> _env;
	public:
		CgiHandler(Request &request);
		~CgiHandler();
		
		std::string execute();
};
