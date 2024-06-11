#pragma once

#include <iostream>

class Reponse
{
	private:

	public:
		Reponse(){};
		~Reponse(){};

		void check_ext_cgi(std::string uri);
};