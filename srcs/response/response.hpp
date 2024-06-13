#pragma once
#include "../errors/ErrorWebServ.hpp"
#include "../server/Server.hpp"
#include <iostream>
#include <string>
#include <iomanip>

class Response
{
public:
	// Default constructor
	Response();
	// Copy constructor
	Response(const Response &other);
	// Copy assignment operator
	Response &operator=(const Response &other);
	// Destructor
	~Response();

	// GETTERS
	std::string getHTTPVersion();
	int getStatusCode();
	std::string getStatusMessage();
	std::map<std::string, std::string> getHeaders();
	std::string getBody();
	std::string getResp();

	// SETTERS
	void setHTTPVersion(std::string version);
	void setStatusCode(int code);
	void setStatusMessage(std::string message);
	void setHeaders(std::istringstream& ss);
	void setBody(std::string body);

	void printResponse() const;
	void formatResponse();

private:
	std::string _httpVersion;
	int _statusCode;
	std::string _statusMessage;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _resp;

	class settingHeadersError : public ErrorWebServ
	{
	public:
		settingHeadersError(int code) : ErrorWebServ(code) {}
		const char *what() const throw();
	};
};