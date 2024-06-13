# include "path.hpp"

void Path::path(Request &a)
{
	Redirection rep;
	if (checkfolder(a.getURI()))
		folderpath(a, rep);
	else
		rep.check_ext_cgi(a.getURI());
}

void Path::folderpath(Request &a, Redirection &rep)
{
	if (a.getCurr_loc().getIndex().empty())
	{
		if (a.getCurr_loc().getAutoindex() == true)
		{
			AutoIndex index;
			// rep.setBody(index.create(a.getFullPath()));
		}
		else
		{
			std::cerr << "Error 403" << std::endl;
			throw std::exception();		//FIXME: Mettre bon throw error
		}

	}
	else
	{
		std::string path = a.getURI() + a.getCurr_loc().getIndex();
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
