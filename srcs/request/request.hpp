#pragma once
#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>

class Request
{
public:
	Request();
	~Request();
	void parseRequest(std::string requestFile);
	void printRequest();

private:
	std::string method;
	std::string uri;
	std::string httpVersion;
	std::map<std::string, std::string> headers;
	std::string body;

	void parseRequestLine(std::string line);
	void parseHeaders(std::string line);
	void parseBody(std::istringstream &ss);
	void parseChunkedBody(std::istringstream &ss);

	class bodySize : public std::exception
	{
		const char *what() const throw();
	};
	class wrongRLInput : public std::exception
	{
		const char *what() const throw();
	};
	class unauthorizedMethod : public std::exception
	{
		const char *what() const throw();
	};
	class headerParsingError : public std::exception
	{
		const char *what() const throw();
	};
	class invalidContentLength : public std::exception
	{
		const char *what() const throw();
	};
	class shorterBodyContent : public std::exception
	{
		const char *what() const throw();
	};

	class longerBodyContent : public std::exception
	{
		const char *what() const throw();
	};
	class contentLengthUnspecified : public std::exception
	{
		const char *what() const throw();
	};
};