#ifndef LISTENSOCKET_HPP
# define LISTENSOCKET_HPP
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <vector>
# include <stdio.h>
# include <cstdlib>
# include <stdlib.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include "../parsing/configserv.hpp"
# define MESSAGE_BUFFER 40000
# define MAX_CLIENTS 30

class configserv;

class ListenSocket {
	private :
		int							_listen_fd;
		int							_binder;
		int							_listener;
		int							_setsockopter;
		int							_activity_mon;
		std::string					_name;
		std::string					_host;
		std::string					_root;
		int							_port;
		struct sockaddr_in			_address;

		ListenSocket(void);

	public :
		ListenSocket(configserv config_serv);
		~ListenSocket(void);
		ListenSocket(const ListenSocket& copy);

		ListenSocket&			operator=(const ListenSocket& ref);

		void					initSocket(void);
		void					run_server(void);

		int						get_listen_fd(void);
		int						get_binder(void);
		int						get_listener(void);
		int						get_setsockopter(void);
		int						get_activity_mon(void);
		struct sockaddr_in		get_address(void);

};

		void					test_connection(int value);
#endif
