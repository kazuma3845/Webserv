#ifndef SERVER_HPP
# define SERVER_HPP
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
# include "../parsing/configserv.hpp"
# include "../client/Client.hpp"
# include "ListenSocket.hpp"

class ListenSocket;
class Client;

class Server {
	private :
		std::vector<ListenSocket>	_listen_sockets;
		std::vector<Client>			_clients;
		// std::map<int, ListenSocket>	_listen_sd_map;
		std::map<int, Client>		_client_sd_map;
		fd_set						_read_sds;
		// fd_set						_write_sds;
		int							_max_sd;
		Server(void);
	public :
		Server(std::vector<configserv>	configservs);
		~Server(void);
		Server(const Server& copy);

		Server&			operator=(const Server& ref);

		void					test_connection(int value);

		void					set_server(void);
		void					run_server(void);
		void					add_client(ListenSocket listen_socket);
		void					read_socket(Client client);
};

#endif