#pragma once

#include "Request.hpp"
#include <iostream>

class Path
{
	private:

	public:
		Path();
		~Path();
		void path(Request &a);
		void folderpath(Request &a);
};
