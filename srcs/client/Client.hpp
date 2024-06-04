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

class Server;

class Client {
	private :
		int							_connected_fd;
		ListenSocket*				_listen_socket;
		Client(void);
	public :
		Client(int fd, ListenSocket &listen_socket);
		~Client(void);
		Client(const Client& copy);

		Client&						operator=(const Client& ref);
		int							get_fd(void);
};

#endif