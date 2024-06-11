#ifndef SERVER_HPP
# define SERVER_HPP

class Server;

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
# include <iomanip>
# include "../parsing/configserv.hpp"
# include "../client/Client.hpp"
# include "ListenSocket.hpp"

class Server {
	private :
		std::vector<ListenSocket>	_ListenSockets;
		std::vector<Client>			_Clients;
		// std::map<int, ListenSocket>	_listen_sd_map;
		std::map<int, Client>		_client_sds_map;
		fd_set						_read_sds;
		fd_set						_write_sds;
		int							_max_sd;
		Server(void);
	public :
		Server(std::vector<configserv>	configservs);
		~Server(void);
		Server(const Server& copy);

		Server&			operator=(const Server& ref);
		void					set_server(void);
		void					run_server(void);
		void					add_client(ListenSocket listen_socket);
		void					read_socket(Client client);
		void					write_socket(Client client);
};

#endif