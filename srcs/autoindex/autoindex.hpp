#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctime>
#include <fstream>

class AutoIndex
{
	private:
		std::string addLine(const std::string& name, const std::string& path);
   		std::string formatTime(time_t rawTime);
    	std::string formatSize(off_t size);
	public:
		AutoIndex(){};
		~AutoIndex(){};

		std::string create(const std::string& uri);
};
