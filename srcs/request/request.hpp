#pragma once
#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>

class Request;
// class Client;

#include "../parsing/location.hpp"
#include "../client/Client.hpp"
#include "../errors/ErrorWebServ.hpp"
class Request
{
public:

	// Request();
	Request(Client &client);
	~Request();
	void parseRequest(std::string requestFile);
	void printRequest();
	void isMethodAllowed();
	void redirectInURI();

	// GET
	std::string getMethod();
	std::string getURI();
	std::string getHttpVersion();
	std::string getBody();
	std::map<std::string, std::string> getHeaders();
	std::string getFullPath();
	location getCurr_loc();

private:
	std::string _method;
	std::string _uri;
	std::string _httpVersion;
	std::map<std::string, std::string> _headers;
	std::string _body;

	// REQUEST PARSING
	void parseRequestLine(std::string line);
	void parseHeaders(std::string line);
	void parseBody(std::istringstream &ss);
	void parseChunkedBody(std::istringstream &ss);

	//LOCATION
	location						_curr_loc;
	std::map<int, std::string>		_map_folders;
	std::string						_file_name;
	std::string						_file_path;
	std::string						_full_path;
	Client&							_client;

	void parseUri(void);

	// ERROR
	class bodySize : public ErrorWebServ
	{
	public:
		bodySize(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class wrongRLInput : public ErrorWebServ
	{
	public:
		wrongRLInput(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class unauthorizedMethod : public ErrorWebServ
	{
	public:
		unauthorizedMethod(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class headerParsingError : public ErrorWebServ
	{
	public:
		headerParsingError(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class invalidContentLength : public ErrorWebServ
	{
	public:
		invalidContentLength(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class shorterBodyContent : public ErrorWebServ
	{
	public:
		shorterBodyContent(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};

	class longerBodyContent : public ErrorWebServ
	{
	public:
		longerBodyContent(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class contentLengthUnspecified : public ErrorWebServ
	{
	public:
		contentLengthUnspecified(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
};