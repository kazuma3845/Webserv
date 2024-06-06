#include "request.hpp"

Request::Request()
{
	std::cout << "Request instance created." << std::endl;
}

Request::~Request()
{
	std::cout << "Request instance destroyed ..." << std::endl;
}

void Request::printRequest()
{
	std::cout << "Method : " << method << std::endl;
	std::cout << "URI : " << uri << std::endl;
	std::cout << "Version :" << httpVersion << std::endl;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		std::cout << it->first << ":" << it->second << std::endl;
	if (!body.empty())
		std::cout << body << std::endl;
}

void Request::parseRequestLine(std::string line)
{
	std::istringstream ss(line);

	ss >> method >> uri >> httpVersion;

	if (method.empty() || uri.empty() || httpVersion.empty())
		throw wrongRLInput();
	// if (AllowdMethods != method) //A modifier pour checker les methodes autorisées par la config du serveur
	// 	throw unauthorizedMethod();
}
void Request::parseHeaders(std::string line)
{
	std::istringstream ss(line);
	std::string key, value;

	if (std::getline(ss, key, ':') && std::getline(ss, value))
		headers[key] = value;
	else
		throw headerParsingError();
}

void Request::parseBody(std::istringstream &ss)
{
	std::stringstream to_convert(headers["content-length"]); // Il se peut qu'il n'y ait pas de content length ...
	int length;
	to_convert >> length;												// conversion en int en utilisant le stream
	std::streamsize buffer_size = static_cast<std::streamsize>(length); // cast pour pouvoir le passer a read
	char buffer[buffer_size + 1];

	// lecture du nombre d'octets definis en amont
	ss.read(buffer, buffer_size);
	std::size_t bytes_read = ss.gcount(); // nombre de bytes reelement lus
	buffer[bytes_read] = '\0';

	body.append(&buffer[0], bytes_read); // conversion et placement dans le body
}

void Request::parseRequest(std::string requestFile)
{
	std::istringstream ss(requestFile);
	std::string line;
	try
	{
		// extraction de la première ligne qui est la RequestLine
		std::getline(ss, line);
		parseRequestLine(line);

		// lecture des headers jusqu'à /r
		while (std::getline(ss, line) && line != "\r") //"\r" fais séparation entre les headers et le body
			parseHeaders(line);

		// lecture du body si il reste qqchose dans le stringstream
		// if (!ss.eof())
		// parseBody(ss);
	}
	catch (std::exception &e)
	{
		std::cout << "Request parsing error : " << e.what() << std::endl;
	}
}

const char *Request::bodySize::what() const throw()
{
	return ("Body length exceeds server limits.");
}

const char *Request::wrongRLInput::what() const throw()
{
	return ("Request line arguments doesn't fit format.");
}

const char *Request::unauthorizedMethod::what() const throw()
{
	return ("Unauthorized method requested.");
}

const char *Request::headerParsingError::what() const throw()
{
	return ("Header parsing error.");
}