# include "Path.hpp"

void Path::path(Request &a)
{
	Reponse rep;
	if (checkfolder(a.getUri()))
		folderpath(a, rep);
	else
		rep.check_ext_cgi(a.getUri());
}

void Path::folderpath(Request &a, Reponse rep)
{
	if (a.getCurr_loc().getIndex().empty())
	{
		if (a.getCurr_loc().getAutoindex() == true)
		{
			// AutoIndex index;
			// file = index.creat(path);
			std::cout << "AutoIndex ON" << std::endl;
		}
		else
		{
			//path = ./Page/error/400/403.html
			std::cerr << "Error 403" << std::endl;
		}

	}
	else
	{
		std::string path = a.getUri() + a.getCurr_loc().getIndex();
		rep.check_ext_cgi(path);
	}
}
		

bool Path::checkfolder(std::string uri)
{
	for (unsigned int i = uri.size(); i != 0; i--)
	{
		if (uri[i] == '.')
			return false;
		if (uri[i] == '/')
			return true;
	}
	return false;
}
