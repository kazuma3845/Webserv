#pragma once

class Reponse;

#include <iostream>
#include <ctime>
#include <cstring>
# include "path.hpp"
#include "../request/request.hpp"
#include "../CgiHandler/CgiHandler.hpp"
# include "../client/Client.hpp"

class Reponse
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
		Reponse(){};
		~Reponse(){};

		//reponse
		void reponseError();
		void reponseMime(Request &a);
		void reponseCGI(Request &a);

		void callPath(Request &req,  Client &client);

		void check_ext_cgi(std::string uri, Client &client);

		//get
		std::string getRep() const;

		//set
		void setBody(std::string newRep);
};