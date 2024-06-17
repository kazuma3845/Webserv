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

class Client {
	private :
		int									_connected_sd;
		ListenSocket						_listen_socket;
		std::string							_rep;
		time_t								_connected_time;
		std::map<std::string, std::string>	_headers;
	public :
		Client();
		Client(int fd, ListenSocket &listen_socket);
		~Client(void);
		// Client(const Client& copy);

		Client&						operator=(const Client& ref);

		void						setResp(std::string rep);
		std::string					getResp() const;
		int							get_fd(void);
		std::string					get_request_content(void);
		ListenSocket				get_listen_socket(void);
		time_t						get_connected_time(void);
		std::map<std::string, std::string> getHeaders();

		void						setHeaders(const std::map<std::string, std::string>& headers);

};
