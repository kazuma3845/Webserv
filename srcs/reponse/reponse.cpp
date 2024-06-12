#include "reponse.hpp"

void Reponse::check_ext_cgi(std::string uri)
{
	Request a;
	if (!a.getCurr_loc().getCgiPath().empty() && checkCgiExt(uri, a))
	{
		std::cout << "CGI" << std::endl;
		reponseCGI(a);
	}
	else if (checkMimeExt(uri))
	{
		std::cout << "MIME" << std::endl;
		reponseMime(a);
	}
	else
	{
		std::cerr << "Error: 415" << std::endl;
		throw std::exception();
	}

}

bool Reponse::checkCgiExt(std::string uri, Request a)
{
	std::string ext = uri.substr(uri.find('.'), uri.size());
	std::vector<std::string> cgiext = a.getCurr_loc().getCgiExt();
	if (cgiext.empty())
		return false;
	for (unsigned int i = 0; i < a.getCurr_loc().getCgiExt().size(); i++)
	{
		if (ext.compare(cgiext[i]) == 0)
			return true;
	}
	return false;
}

bool Reponse::checkMimeExt(std::string uri)
{
	std::string ext = uri.substr(uri.find('.'), uri.size());
	std::string mime[77] = {".aac", ".abw", ".apng", ".arc", ".avif", ".avi", ".azw",\
	".bin", ".bmp", ".bz", ".bz2",\
	".cda", ".csh", ".css", ".csv",\
	".doc", ".docx",\
	".eot", ".epub",\
	".gz", ".gif",\
	".htm", ".html"\
	".ico", ".ics",\
	".jar", ".jpeg", ".jpg", ".js", ".json", ".jsonld",\
	".mid", ".midi", ".mjs", ".mp3", ".mp4", ".mpeg", ".mpkg",\
	".odp", ".ods", ".odt", ".oga", ".ogv", ".ogx", ".opus", ".otf",\
	".png", ".pdf", ".php", ".ppt", ".pptx",\
	".rar", ".rtf",\
	".sh", ".svg",\
	".tar", ".tif", ".tiff", ".ts", ".ttf", ".txt",\
	".vsd",\
	".wav",	".weba", ".webm", ".webp", ".woff", ".woff2",\
	".xhtml", ".xls", ".xlsx", ".xml", ".xul",\
	".zip", ".3gp", ".3g2", ".7z"};
	for (unsigned int i = 0; i < 77; i++)
	{
		if (ext.compare(mime[i]) == 0)
			return true;
	}
	return false;
}

void Reponse::reponseMime(Request &a)
{
	(void)a;
}

void Reponse::reponseCGI(Request &a)
{
	CgiHandler cgi(a);
	_reponse_html =	"HTTP/1.1 200 ok\r\n"
					"Date: " + takeTime() + "\r\n"
					"Content-Type: text/html\r\n"
					"\r\n"
					"<html><body>"
					"<h1>" + takeTime() + "</h1>"
					// FIXME: "<h1>" + cgi.execute() + "</h1>"
					"</body></html>";
}

void Reponse::reponseError()
{

}

std::string Reponse::getRep() const
{
	return _reponse_html;
}

std::string Reponse::takeTime()
{
	time_t raw_time;
    struct tm *time_info;
    char buffer[80];

    time(&raw_time);
    time_info = localtime(&raw_time);

    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %Z", time_info);
	std::string str_buffer(buffer);
	return str_buffer;
}

void Reponse::callPath(Request &req)
{
	Path a;
	a.path(req);
}

void Reponse::setBody(std::string newRep)
{
	_body = newRep;
}
