/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleSocket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasoni <mmasoni@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 14:49:17 by mmasoni           #+#    #+#             */
/*   Updated: 2024/04/25 13:04:42 by mmasoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLESOCKET_HPP
# define SIMPLESOCKET_HPP
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <vector>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <fcntl.h>

# define MESSAGE_BUFFER 40000
#	define MAX_CLIENTS 30

class SimpleSocket {
	private :
		int					_server_fd;
		int					_binder;
		int					_listener;
		int					_setsockopter;
		int					_activity_mon;
		// fd_set				_read_fds;
		std::vector<int>	_client_fds;

		struct sockaddr_in	_address;
		SimpleSocket(void);

	public :
		SimpleSocket(int domain, int type, int protocol, int port, u_long interface_IP);
		~SimpleSocket(void);
		SimpleSocket(const SimpleSocket& copy);

		SimpleSocket&			operator=(const SimpleSocket& ref);

		void					test_connection(int value);
		void					run_server(void);

		int						get_server_fd(void);
		int						get_binder(void);
		int						get_listener(void);
		int						get_setsockopter(void);
		int						get_activity_mon(void);
		struct sockaddr_in		get_address(void);

};

#endif
