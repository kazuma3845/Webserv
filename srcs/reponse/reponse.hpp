#pragma once

#include <iostream>
// #include "request.hpp"

class Reponse
{
	private:

	public:
		Reponse(){};
		~Reponse(){};

		void check_ext_cgi(std::string uri);
};