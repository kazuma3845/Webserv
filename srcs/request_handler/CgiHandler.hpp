#pragma once

#include <map>
#include <iostream>
#include "../request/request.hpp"

#define BUFFER 1000

class CgiHandler
{
	private:
		std::string _body;
		std::map<std::string, std::string> _env;

		void initenv(Request &request);
		char **EnvToArray() const;
	public:
		CgiHandler(Request &request);
		~CgiHandler();
		
		std::string execute(std::string Script);

        class InternalServerError : public ErrorWebServ
        {
            public:
                InternalServerError(int errorCode) : ErrorWebServ(errorCode) {}
                const char *what() const throw();
        };
};
