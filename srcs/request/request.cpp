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
	std::cout << "Version : " << httpVersion << std::endl;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		std::cout << it->first << " : " << it->second << std::endl;
	if (!body.empty())
		std::cout << "Body : " << body << std::endl;
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
	{
		size_t start = value.find_first_not_of(" \t\r\n");
		if (start != std::string::npos)
			value.erase(0, start);
		size_t end = value.find_last_not_of(" \t\r\n");
		if (end != std::string::npos)
			value.erase(end + 1);
		headers[key] = value;
	}
	else
		throw headerParsingError();
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
	body = bodyStream.str();
	// std::cout << "Chunked body read successfully: " << body << std::endl;
}

void Request::parseBody(std::istringstream &ss)
{

	if (headers.find("Content-Length") == headers.end())
		throw contentLengthUnspecified();
	std::stringstream to_convert(headers["Content-Length"]);
	int length;
	to_convert >> length;
	if (length <= 0)
		throw invalidContentLength();
	char *buffer = new char[length + 1];
	ss.read(buffer, length);
	buffer[ss.gcount()] = '\0';
	body.assign(buffer, ss.gcount());
	if (ss.gcount() != length)
		throw shorterBodyContent();
	if (ss.get() != EOF)
		throw longerBodyContent();
	delete[] buffer;
	// std::cout << "Body read successfully: " << body << std::endl;
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
		while (std::getline(ss, line) && line != "\r" && line != "\r\n" && line != "\n" && !line.empty()) //"\r" fais séparation entre les headers et le body
			parseHeaders(line);

		// lecture du body si il reste qqchose dans le stringstream
		char next_char = ss.get();
		ss.putback(next_char);
		if (next_char != EOF)
		{
			if (headers.find("Transfer-Encoding") != headers.end() && headers["Transfer-Encoding"] == "chunked")
				parseChunkedBody(ss);
			else
				parseBody(ss);
		}
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
