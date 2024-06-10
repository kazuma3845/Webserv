# include "Path.hpp"

void Path::path(Request &a)
{
	if (a.url() == folder)
		folderpath(a);
	else
		cgi_ext(a.url());
}

void Path::folderpath(Request &a)
{
	if (a.getIndex().empty())
	{
		if (b.getAutoindex() == true)
			takefile();
		else
			takeerror(403);
	}
	else
		cgi_ext(a.url());
}
