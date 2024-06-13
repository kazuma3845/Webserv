#pragma once

class Redirection;

#include <iostream>
#include <ctime>
#include <cstring>
# include "path.hpp"
#include "../request/request.hpp"
#include "CgiHandler.hpp"
# include "../client/Client.hpp"

class Redirection
{
	private:
		std::string _reponse_html;
		std::string _body;

		//check
		bool checkCgiExt(std::string uri, Request a);
		bool checkMimeExt(std::string uri);

		//additional
		std::string takeTime();
	public:
		Redirection(){};
		~Redirection(){};

		void reponseCGI(Request &a);
		void callPath(Request &req,  Client &client);
		void check_ext_cgi(std::string uri, Client &client);

		//get
		std::string getRep() const;
};