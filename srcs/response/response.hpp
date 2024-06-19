#pragma once
#include "../errors/ErrorWebServ.hpp"
#include "../request/Client.hpp"
#include "../request/request.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

class Response
{
public:
	Response();
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	// ----------------------------------- GETTERS
	std::string getHTTPVersion();
	int getStatusCode();
	std::string getStatusMessage();
	// std::map<std::string, std::string> getHeaders();
	std::string getBody();
	std::string getResp();

	// ---------------------------------- SETTERS
	void setHTTPVersion(std::string version);
	void setStatusCode(int code);
	void setStatusMessage(std::string message);
	// void setHeaders(std::istringstream &ss);
	void setBody(std::string body);
	void setContentType(std::string type);
	void setConnectionType(bool status);

	// ------------------------------------ FUNCTIONS
	void printResponse() const;
	void formatResponse(Client &a, Request &b);
	void loadContent(const std::string &filePath);

	void ErrorBody(int error_code);

private:
	// --------------------------------------- ATTRIBUTES
	std::string _httpVersion;
	int _statusCode;
	std::string _statusMessage;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _resp;
	std::string _contentType;
	std::string _connectionType;

	std::string takeTime() const;

	// -------------------------------------- ERRORS
	class settingHeadersError : public ErrorWebServ
	{
	public:
		settingHeadersError(int code) : ErrorWebServ(code) {}
		const char *what() const throw();
	};

		class cantLoadContent : public ErrorWebServ
	{
	public:
		cantLoadContent(int code) : ErrorWebServ(code) {}
		const char *what() const throw();
	};
};
