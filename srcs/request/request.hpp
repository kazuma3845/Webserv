#pragma once
#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>
#include <sys/stat.h>

#include <cstring>

class Request;

#include "../parsing/location.hpp"
#include "../errors/ErrorWebServ.hpp"
#include "Client.hpp"

enum parsingStatus {
	PARSING_RL,
	PARSING_HEADERS,
	PARSING_BODY,
	PARSING_FINISHED,
};

class Request
{
public:
	Request();
	Request(Client *client);
	Request(const Request &other);
	Request& operator=(const Request& other);
	~Request();
	void parseRequest(int fd);
	void printRequest();
	void checkRequest();

	// ------------------------------------------- GETTERS

	std::string getMethod();
	std::string getURI();
	std::string getHttpVersion();
	std::string getBody();
	std::map<std::string, std::string> getHeaders();
	std::string getFilePath();
	std::string getFullPath();
	location getCurr_loc();
	std::string getQueryString();
	Client *getClient();
	bool getHasReturn();

	// ------------------------------------------- SETTERS

	void setURI(std::string uri);
	void setLocation(location &loc);
	void setFilePath(std::string filePath);
	void setFullPath(std::string fullPath);
	void setQueryString(std::string queryString);
	void setClient(Client *client);
	void setStatus(parsingStatus status);
	void parseBody(int fd);

private:
	// ------------------------------------------ ATTRIBUTES

	std::string _method;
	std::string _uri;
	std::string _httpVersion;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _queryString;
	location _curr_loc;
	std::string _file_path;
	std::string _full_path;
	bool _hasReturn;
	Client *client;
	parsingStatus status;
	std::string _buffer;
	unsigned int _chunkBodySize;
	std::string _chunkBuffer;

	// ------------------------------------------ REQUEST PARSING FUNCTIONS

	void prepareBodyParsing(int fd);
	std::string parseRequestLine(std::string &current_buffer);
	std::string parseHeaders(std::string &current_buffer);
	void processChunkedBody(int fd);
	void extractQueryString();
	void isMethodAllowed();
	void redirectInURI();
	void parseUri();
	void checkFile(int mode);
	bool checkfolder(std::string uri);
	std::string cleanString(std::string toClean);
	void ChunkedBody(std::string &current_buffer);
	void Body(std::string current_buffer);

	// ------------------------------------------ ERROR

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
	class unauthorizedMethod : public ErrorWebServ
	{
	public:
		unauthorizedMethod(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class fileNotFound : public ErrorWebServ
	{
	public:
		fileNotFound(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class unsupportedHTTPVersion : public ErrorWebServ
	{
	public:
		unsupportedHTTPVersion(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class chunkSizeError : public ErrorWebServ
	{
	public:
		chunkSizeError(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class chunkDataError : public ErrorWebServ
	{
	public:
		chunkDataError(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class connectionCloseEarly : public ErrorWebServ
	{
	public:
		connectionCloseEarly(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
	class errorReadingFD : public ErrorWebServ
	{
	public:
		errorReadingFD(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
};

void replaceDoubleSlashes(std::string &str);
bool isDirectory(std::string &path);
