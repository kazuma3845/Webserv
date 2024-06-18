#include "request.hpp"

// ------------------------------------ COPLIEN -- //

Request::Request()
{
	// std::cout << "Request instance created." << std::endl;
}

Request::Request(Client *client) : client(client)
{
	// std::cout << "Request instance with pointer on client created." << std::endl;
}

Request::~Request()
{
	// std::cout << "Request instance destroyed ..." << std::endl;
}
// ---------------------------------- GETTERS -- //

std::string Request::getMethod()
{
	return (_method);
}

std::string Request::getURI()
{
	return (_uri);
}

std::string Request::getHttpVersion()
{
	return (_httpVersion);
}

std::string Request::getBody()
{
	return (_body);
}

std::map<std::string, std::string> Request::getHeaders()
{
	return (_headers);
}

location Request::getCurr_loc()
{
	return _curr_loc;
}

std::string Request::getFilePath()
{
	return (_file_path);
}

std::string Request::getFullPath()
{
	return (_full_path);
}

std::string Request::getQueryString()
{
	return (_queryString);
}

// ---------------------------------- SETTERS -- //

void Request::setURI(std::string uri)
{
	_uri = uri;
}
void Request::setLocation(location &loc)
{
	_curr_loc = loc;
}

void Request::setFilePath(std::string filePath)
{
	_file_path = filePath;
}

void Request::setFullPath(std::string fullPath)
{
	_full_path = fullPath;
}

void Request::setQueryString(std::string queryString)
{
	_queryString = queryString;
}

void Request::setClient(Client *client)
{
	this->client = client;
}
// ---------------------------------- OTHER FUNCTIONS -- //

void Request::printRequest()
{
	std::cout << std::endl
			  << "Method : " << _method << std::endl;
	std::cout << "URI : " << _uri << std::endl;
	std::cout << "Version : " << _httpVersion << std::endl;
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
		std::cout << it->first << " : " << it->second << std::endl;
	if (!_body.empty())
		std::cout << "Body : " << _body << std::endl;
	if (!_curr_loc.empty())
		std::cout << "Location : " << _curr_loc.getName() << std::endl;
	std::cout << "File_path : " << _file_path << std::endl;
	std::cout << "Full_path : " << _full_path << std::endl;
	if (!_queryString.empty())
		std::cout << "Query String : " << _queryString << std::endl;
}

void Request::extractQueryString()
{
	size_t pos = _uri.find('?');
	std::string tmp = _uri.substr(0, pos);
	_queryString = _uri.substr(pos + 1);
	_uri = tmp;
}

void Request::parseRequestLine(std::string line)
{
	std::istringstream ss(line);

	ss >> _method >> _uri >> _httpVersion;

	if (_uri.find('?') != std::string::npos)
		extractQueryString();
	char extra;
	if (_method.empty() || _uri.empty() || _uri.front() != '/' || _httpVersion.empty() || _httpVersion.substr(0, 5) != "HTTP/" || ss >> extra)
		throw wrongRLInput(400);
}
void Request::parseHeaders(std::string line)
{
	std::istringstream ss(line);
	std::string key, value;

	if (std::getline(ss, key, ':') && std::getline(ss, value))
	{
		size_t start = value.find_first_not_of(" \t\r\n");
		if (start != std::string::npos)
			value.erase(0, start);
		size_t end = value.find_last_not_of(" \t\r\n");
		if (end != std::string::npos)
			value.erase(end + 1);
		_headers[key] = value;
	}
	else
		throw headerParsingError(400);
}

void Request::parseChunkedBody(std::istringstream &ss)
{
	std::string line;
	std::ostringstream bodyStream;

	while (std::getline(ss, line))
	{
		std::stringstream lineStream(line);
		int length;
		lineStream >> std::hex >> length; // Lire la taille en hexadécimal
		if (length == 0)
			break;
		std::vector<char> buffer(length);
		ss.read(&buffer[0], length);
		bodyStream.write(&buffer[0], length);
		std::getline(ss, line);
	}
	_body = bodyStream.str();
	// std::cout << "Chunked body read successfully: " << _body << std::endl;
}

void Request::parseBody(std::istringstream &ss)
{

	if (_headers.find("Content-Length") == _headers.end())
		throw contentLengthUnspecified(411);
	std::stringstream to_convert(_headers["Content-Length"]);
	int length;
	to_convert >> length;
	if (length <= 0)
		throw invalidContentLength(400);
	char *buffer = new char[length + 1];
	ss.read(buffer, length);
	buffer[ss.gcount()] = '\0';
	_body.assign(buffer, ss.gcount());
	if (ss.gcount() != length)
		throw shorterBodyContent(400);
	if (ss.get() != EOF)
		throw longerBodyContent(400);
	if (client->get_listen_socket().get_clientSize() > std::strtoul(_headers["Content-Length"].c_str(), NULL, 10))
		throw bodySize(413);
	delete[] buffer;
	// std::cout << "Body read successfully: " << _body << std::endl;
}

void Request::parseRequest(std::string requestFile)
{
	std::istringstream ss(requestFile);
	std::string line;

	// extraction de la première ligne qui est la RequestLine
	std::getline(ss, line);
	parseRequestLine(line);

	// lecture des headers jusqu'à /r
	while (std::getline(ss, line) && line != "\r" && line != "\r\n" && line != "\n" && !line.empty()) //"\r" fais séparation entre les headers et le body
		parseHeaders(line);

	// lecture du body si il reste qqchose dans le stringstream
	char next_char = ss.get();
	ss.putback(next_char);
	if (next_char != EOF)
	{
		if (_headers.find("Transfer-Encoding") != _headers.end() && _headers["Transfer-Encoding"] == "chunked")
			parseChunkedBody(ss);
		else
			parseBody(ss);
	}
	// * CHECKING REQUEST //
	parseUri(); // Get location from URI
	if (!_headers["Referer"].empty())
	{
		if (_headers["Referer"].back() != '/')
			_uri = _headers["Referer"].substr(21, _headers["Referer"].size()) + _uri;
	}
	parseUri();		   // Get location from URI
	isMethodAllowed(); // Check that method called is allowed in the directory
	redirectInURI();   // Check if there is a return in the directory
}

void Request::parseUri()
{
	int slash_pos;
	int end_pos;
	std::string temp_loc_path;
	std::string temp_file_path;
	std::string uri;

	uri = this->_uri;
	end_pos = sizeof(uri);
	slash_pos = end_pos;
	temp_loc_path = uri;
	while (1)
	{
		for (size_t i = 0; i < client->get_listen_socket().get_location().size(); ++i)
		{
			if (!client->get_listen_socket().get_location()[temp_loc_path].getName().empty())
			{
				this->_curr_loc = client->get_listen_socket().get_location()[temp_loc_path];
				break;
			}
		}
		if (slash_pos <= 0 || !_curr_loc.getName().empty())
			break;
		else
		{
			slash_pos = temp_loc_path.find_last_of('/');
			temp_loc_path = uri.substr(0, slash_pos);
			temp_file_path = uri.substr(slash_pos + 1, end_pos - slash_pos);
			if (slash_pos == 0)
			{
				temp_loc_path = "/";
				temp_file_path = uri.substr(slash_pos + 1, end_pos - slash_pos);
			}
		}
	}
	std::string temp_root = client->get_listen_socket().get_root();
	if (!_curr_loc.getRoot().empty())
		temp_root = _curr_loc.getRoot();
	_file_path = temp_file_path;
	if (!_curr_loc.getIndex().empty() && _file_path.empty())
	{
		_file_path = _curr_loc.getIndex();
		_full_path = temp_root + _curr_loc.getName() + "/" + _file_path;
		if (access(_full_path.c_str(), F_OK) && _curr_loc.getAutoindex())
			_file_path.clear();
	}
	if (_curr_loc.empty())
		_file_path = uri;
	_full_path = temp_root + _curr_loc.getName() + "/" + _file_path;
	replaceDoubleSlashes(_full_path);
	redirectInURI();
}

void Request::checkFile(int mode)
{

	if (access(_full_path.c_str(), mode))
		throw fileNotFound(404);
	return;
}

void Request::isMethodAllowed()
{
	std::vector<std::string> methods;
	if (_curr_loc.empty()) // si il n'y a pas de location, alors on va chercher les allowed method a la racine
		methods = client->get_listen_socket().get_allow_methods();
	else
		methods = _curr_loc.getAllowMethods();

	if (methods.empty())
	{
		std::cout << "LOCATION ASSOCIATED WITH REQUEST IS : " << _curr_loc.getName() << std::endl;
		std::cout << "METHODS ARE EMPTY" << std::endl;
	}
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); ++it)
	{
		if (_method == *it)
		{
			// std::cout << "AUTHORIZED METHOD " << *it << " IN LOCATION : " << _curr_loc.getName() << std::endl;
			return;
		}
		std::cout << _method << " DOES NOT EQUAL TO " << *it << std::endl;
	}
	throw unauthorizedMethod(405);
}

void Request::redirectInURI() // FIXME: A voir si on veut bien remplacer complètement l'URI
{
	std::cout << "Retrun !!! " << _body << std::endl;
	if (_curr_loc.empty())
		return;
	if (!_curr_loc.getReturn().empty())
	{
		setURI(_curr_loc.getReturn());
		_curr_loc = location();
		_file_path.clear();
		_full_path.clear();
		parseUri();
	}
}

// ---------------------------------- ERROR -- //

const char *Request::bodySize::what() const throw()
{
	return ("Body length exceeds server limits.");
}

const char *Request::wrongRLInput::what() const throw()
{
	return ("Request line arguments doesn't fit format.");
}

const char *Request::headerParsingError::what() const throw()
{
	return ("Header parsing error.");
}

const char *Request::invalidContentLength::what() const throw()
{
	return ("Negative length content specified.");
}

const char *Request::shorterBodyContent::what() const throw()
{
	return ("Mismatch between specified content-length and actuel content size (shorter).");
}
const char *Request::longerBodyContent::what() const throw()
{
	return ("Mismatch between specified content-length and actuel content size (longer).");
}
const char *Request::contentLengthUnspecified::what() const throw()
{
	return ("Content-length was not specified.");
}

const char *Request::unauthorizedMethod::what() const throw()
{
	return ("Unauthorized method requested.");
}

const char *Request::fileNotFound::what() const throw()
{
	return ("File not found");
}

void replaceDoubleSlashes(std::string &str)
{
	std::string::size_type pos = 0;
	while ((pos = str.find("//", pos)) != std::string::npos)
	{
		str.replace(pos, 2, "/");
	}
}

Client *Request::getClient()
{
	return client;
}
