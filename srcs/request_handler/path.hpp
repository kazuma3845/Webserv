#pragma once

class Path;

#include "../request/request.hpp"
#include "redirection.hpp"
#include "autoindex.hpp"
# include "../client/Client.hpp"

#include <iostream>

class Path
{
	private:
		bool checkfolder(std::string uri);
	public:
		Path(){};
		~Path(){};
		void path(Request &a,  Client &client);
		void folderpath(Request &a, Redirection &rep, Client &client);
};
