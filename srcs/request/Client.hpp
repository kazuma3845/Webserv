#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "../errors/ErrorWebServ.hpp"

enum Flag
{
	PARSING_REQUEST,
	EXPECTING,
	REDIRECTING,
	HANDLING_REQUEST,
	WRITING_RESPONSE,
	FINISHED
};

class Client;
class Handler;
class Response;

#include "request.hpp"
#include "../server/ListenSocket.hpp"

class Client
{
private:
	// ---------------------------------------------- ATTRIBUTES

	int _connected_sd;
	bool _keepAlive;

	std::string _rep;
	time_t _connected_time;
	std::map<std::string, std::string> _headers;

	ListenSocket _listen_socket;
	Request *_request;
	Response *_response;
	Handler *_handler;

	Flag _flag;

public:
	Client();
	Client(int fd, ListenSocket &listen_socket);
	Client &operator=(const Client &ref);
	~Client();

	void reUseClient(void);
	// --------------------------------------------- GETTERS
	bool getKeepAlive();
	int get_fd(void);

	std::string getResp() const;
	time_t get_connected_time(void);

	ListenSocket get_listen_socket(void);
	Request *getReq(void);
	Handler *getHandler();
	Response *getResponse();
	Flag getFlag();

	// --------------------------------------------- SETTERS
	void setReq(Request *request);
	void setHeaders(const std::map<std::string, std::string> &headers);
	void setKeepAlive(bool status);
	void setResp(std::string rep);
	void setTimeout(void);
	void setFlag(Flag flag);
};
