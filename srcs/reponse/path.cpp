# include "Path.hpp"

void Path::path(Request &a)
{
	if (checkfolder(a.getUri()))
		folderpath(a);
	else
		cgi_ext(a.getUri());
}

void Path::folderpath(Request &a)
{
	if (a.getCurr_loc().getIndex().empty())
	{
		if (a.getCurr_loc().getAutoindex() == true)
			;//reponseliste;
		else
			error(403);
	}
	else
		cgi_ext(a.getUri());
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
