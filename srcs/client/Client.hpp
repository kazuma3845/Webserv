#ifndef CLIENT_HPP
# define CLIENT_HPP
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
# include "../server/ListenSocket.hpp"

#define HTML_CONTENT \
"<!DOCTYPE html>\n" \
"<html>\n" \
"<head>\n" \
"    <title>Webserv</title>\n" \
"    <meta charset=\"UTF-8\">\n" \
"</head>\n" \
"<body>\n" \
"    <img src=\"https://cdn.intra.42.fr/users/cd0bb5b5fef19852f99b709bbceb556c/mmasoni.jpg\" alt=\"Example Image\" style=\"width : 200px;\" >\n" \
"    <h1>Hello, moi c'est Martin Matin !</h1>\n" \
"    <p>Je suis très matinal, sutout pour un valaisan ... 😭</p>\n" \
"</body>\n" \
"</html>\n"

class Server;

class Client {
	private :
		int							_connected_sd;
		ListenSocket*				_listen_socket;
		std::string					_request_content;
	public :
		Client(void);
		Client(int fd, ListenSocket &listen_socket);
		~Client(void);
		Client(const Client& copy);

		Client&						operator=(const Client& ref);
		int							get_fd(void);
		std::string					get_request_content(void);

		void						set_request_content(std::string request_content);
};

#endif