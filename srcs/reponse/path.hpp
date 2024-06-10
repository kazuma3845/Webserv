#pragma once

#include "../request/request.hpp"
#include "reponse.hpp"

#include <iostream>

class Path
{
	private:
		bool checkfolder(std::string uri);
	public:
		Path(){};
		~Path(){};
		void path(Request &a);
		void folderpath(Request &a, Reponse rep);
};
