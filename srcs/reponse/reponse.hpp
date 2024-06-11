#pragma once

#include <iostream>
#include "../request/request.hpp"

class Reponse
{
	private:
		//check
		bool checkCgiExt(std::string uri, Request a);
		bool checkMimeExt(std::string uri);

		//reponse
		void reponseMime();
		void reponseCGI();
	public:
		Reponse(){};
		~Reponse(){};

		void check_ext_cgi(std::string uri);
};