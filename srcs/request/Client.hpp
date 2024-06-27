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
	HANDLING_REQUEST,
	FINISHED
};

class Client;

#include "request.hpp"
#include "../server/ListenSocket.hpp"

class Client
{
private:
	// ---------------------------------------------- ATTRIBUTES
	int _connected_sd;
	ListenSocket _listen_socket;
	std::string _rep;
	time_t _connected_time;
	std::map<std::string, std::string> _headers;
	bool _keepAlive;
	Request* _request;
	Flag _flag;

public:
	Client();
	Client(int fd, ListenSocket &listen_socket);
	~Client();
	Client &operator=(const Client &ref);

	void reUseClient(void);
	// --------------------------------------------- GETTERS
	std::string getResp() const;
	int get_fd(void);
	ListenSocket get_listen_socket(void);
	time_t get_connected_time(void);
	bool getKeepAlive();
	Request *getReq(void);
	Flag getFlag();

	// --------------------------------------------- SETTERS
	void setReq(Request *request);
	void setHeaders(const std::map<std::string, std::string> &headers);
	void setKeepAlive(bool status);
	void setResp(std::string rep);
	void setTimeout(void);
	void setFlag(Flag flag);
};
