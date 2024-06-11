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
class Client;
# include "../server/ListenSocket.hpp"
# include "../request/request.hpp"

#define HTML_CONTENT \
"<!DOCTYPE html>\n" \
"<html>\n" \
"<head>\n" \
"    <title>Webserv</title>\n" \
"    <meta charset=\"UTF-8\">\n" \
"</head>\n" \
"<body>\n" \
"    <img src=\"https://cdn.intra.42.fr/users/b744d09961e78146e695def8611033bb/tomuller.jpg\" alt=\"Example Image\" style=\"width : 200px;\" >\n" \
"    <h1>Hello, moi c'est Tomtom !</h1>\n" \
"    <p>Je rage quand je perds à Mariokart ... 😭</p>\n" \
"</body>\n" \
"</html>\n"

class Client {
	private :
		int							_connected_sd;
		ListenSocket				_listen_socket;
		Request						_request;
	public :
		// Client(void);
		Client(int fd, ListenSocket &listen_socket);
		~Client(void);
		Client(const Client& copy);

		Client&						operator=(const Client& ref);
		int							get_fd(void);
		std::string					get_request_content(void);
		ListenSocket				get_listen_socket(void);
		Request*						get_request(void);
		void						set_request(Request& request);
};
