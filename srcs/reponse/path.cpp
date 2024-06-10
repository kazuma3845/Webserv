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
			std::cout << "AutoIndex ON" << std::endl;
		else
			std::cerr << "Error 403" << std::endl;
	}
	else
		rep.check_ext_cgi(a.getUri());
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
