#pragma once

#include <iostream>
#include "../request/request.hpp"

class Reponse
{
	private:
		std::string reponse_html;

		//check
		bool checkCgiExt(std::string uri, Request a);
		bool checkMimeExt(std::string uri);

		//reponse
		void reponseMime();
		void reponseCGI();
		void reponseError();
	public:
		Reponse(){};
		~Reponse(){};

		void check_ext_cgi(std::string uri);
};