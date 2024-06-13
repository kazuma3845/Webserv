#pragma once
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <vector>
# include <map>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <fcntl.h>
#include "../errors/ErrorWebServ.hpp"

class Client;

# include "../server/ListenSocket.hpp"
# include "request.hpp"

class Client {
	private :
		int							_connected_sd;
		ListenSocket				_listen_socket;
		Request						_request;
		std::string _rep;
	public :
		Client();
		Client(int fd, ListenSocket &listen_socket);
		~Client(void);
		// Client(const Client& copy);

		Client&						operator=(const Client& ref);
		int							get_fd(void);
		std::string					get_request_content(void);
		ListenSocket				get_listen_socket(void);
		Request						get_request(void);
		void isMethodAllowed();
		void redirectInURI();

	class unauthorizedMethod : public ErrorWebServ
	{
	public:
		unauthorizedMethod(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
};
