#pragma once

#include <iostream>
#include <dirent.h>

class AutoIndex
{
	private:

	public:
		AutoIndex(){};
		~AutoIndex(){};

		std::string creat(std::string uri);
};
