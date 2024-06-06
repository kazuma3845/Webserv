// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   ListenSocket.cpp                                         :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: mmasoni <mmasoni@student.42lausanne.ch>    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/04/09 14:49:15 by mmasoni           #+#    #+#             */
// /*   Updated: 2024/04/25 13:06:48 by mmasoni          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "ListenSocket.hpp"

// // ------------------- Constructors -------------------

// ListenSocket::ListenSocket(void)
// {
// 	std::cout << "New ListenSocket called" << std::endl;
// }

// ListenSocket::ListenSocket(configserv configserv)
// {
// 	// Filling Address'struct
// 	this->_name = configserv.get;

// 	std::cout << "ListenSocket was called." << std::endl;

// }

// // ListenSocket::ListenSocket(int domain, int type, int protocol, int port, u_long interface_IP)
// // {
// // 	// Filling Address'struct
// // 	this->_address.sin_family = domain;
// // 	this->_address.sin_addr.s_addr = interface_IP;
// // 	this->_address.sin_port = htons(port);

// // 	// Establish main server socket
// // 	this->_listen_fd = socket(domain, type, protocol);
// // 	test_connection(this->_listen_fd);

// // 	// Initialize all client_fd to 0 so not checked
// // 	this->_client_fds.assign(MAX_CLIENTS, 0);

// // 	// Set socket to allow multiple connections (reuse address and port)
// // 	int opt = 1;
// // 	this->_setsockopter = setsockopt(this->_listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
// // 	test_connection(this->_setsockopter);

// // 	// Bind socket to IP address and port
// // 	this->_binder = bind(this->_listen_fd, (struct sockaddr *)&this->_address, sizeof(this->_address));
// // 	test_connection(this->_binder);

// // 	// Set a listener ont the socket
// // 	this->_listener = listen(this->_listen_fd, 512);
// // 	test_connection(this->_listener);

// // 	std::cout << "ListenSocket was called." << std::endl;

// // }

// ListenSocket::~ListenSocket(void)
// {
// 	std::cout << "ListenSocket was destroyed." << std::endl;
// }

// ListenSocket::ListenSocket( const ListenSocket& copy )
// {
// 	std::cout << "ListenSocket copy constructor called" << std::endl;
// 	*this = copy;
// }

// ListenSocket& ListenSocket::operator=( const ListenSocket& ref )
// {
// 	std::cout << "ListenSocket assignment operator called" << std::endl;
// 	if ( this != &ref )
// 	{
// 		this->_listen_fd = ref._listen_fd;
// 		this->_binder = ref._binder;
// 		this->_listener = ref._listener;
// 		this->_setsockopter = ref._setsockopter;
// 		this->_activity_mon = ref._activity_mon;
// 		this->_client_fds = ref._client_fds;
// 		this->_address = ref._address;
// 	}
// 	return *this;
// }

// void ListenSocket::test_connection(int value)
// {
// 	if (value < 0)
// 	{
// 		std::cerr << "A connection error occured" << std::endl;
// 		exit(EXIT_FAILURE);
// 	}
// }

// void ListenSocket::run_server(void)
// {
// 	fd_set			read_fds;
// 	int				sd; /* Socket descriptor */
// 	int				max_sd; /* Max Socket descriptor */
// 	int				new_socket;
// 	int				non_block;
// 	int				addrlen;
// 	char			buffer[MESSAGE_BUFFER];

// 	std::cout  << std::endl << "SERVER STARTED" << std::endl;
// 	while (1) {
// 		std::cerr << "|START" << std::endl;
// 		// Clear the socket set to void
// 		FD_ZERO(&read_fds);

// 		// Add master socket to set
// 		FD_SET(this->_listen_fd, &read_fds);

// 		// Save the max socket descriptor for forther use
// 		max_sd = this->_listen_fd;

// 		// Add child sockets to set
// 		for (int i = 0; i < MAX_CLIENTS; i++) {
// 			if (sd == 0)
// 				continue ;
// 			sd = this->_client_fds[i];
// 			FD_SET(sd, &read_fds);  /* If valid socket descriptor then add to read list */
// 			if (sd > 0) {
// 				FD_SET(sd, &read_fds);
// 			}
// 			if (sd > max_sd) /* Highest file descriptor number for select function */
// 				max_sd = sd;
// 		}

// 		// Wait for an activity on one of the , select return the value of readies FD
// 		this->_activity_mon = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
// 		test_connection(this->_activity_mon);

// 		// If something happened on the master socket, it's an incoming connection
// 		if (FD_ISSET(this->_listen_fd, &read_fds))
// 		{
// 			addrlen = sizeof(this->_address);
// 			new_socket = accept(this->_listen_fd, (struct sockaddr *)&this->_address, (socklen_t*)&addrlen);
// 			test_connection(new_socket);

// 			// Set new socket as non blocked
// 			non_block = fcntl(new_socket, F_SETFL, O_NONBLOCK);
// 			test_connection(non_block);

// 			// ------------- Message for testing
// 			std::cout << "|   New connection: socket fd is " << new_socket << " | " << inet_ntoa(this->_address.sin_addr) << " | "<< ntohs(this->_address.sin_port) << std::endl;

// 			// Add new socket to array of sockets
// 			for (int i = 0; i < MAX_CLIENTS; i++) 
// 			{
// 				if (this->_client_fds[i] == 0)
// 				{
// 					std::cerr << "|       new_socket added : "<< new_socket << std::endl;
// 					FD_SET(new_socket, &read_fds);
// 					this->_client_fds[i] = new_socket;
// 					break;
// 				}
// 			}
// 		}

// 		// Else it's some IO operation on some other socket and we check every sd
// 		for (int i = 0; i < MAX_CLIENTS; i++) {
// 			sd = this->_client_fds[i];
// 			if (FD_ISSET(sd, &read_fds)) {
// 				std::cerr << "|      ----> Socket " << sd << " was readed and will close" << std::endl;

// 				// Handle the stuff to do late .....
// 				memset(buffer, 0, sizeof(buffer));
// 				int has_content = read(sd, buffer, MESSAGE_BUFFER);
// 				if (has_content)
// 				{
// 					std::cerr << "-------------- CONTENT --------------" << std::endl;
// 					std::cerr << buffer << std::endl;
// 					std::cerr << "-------------- ------- --------------" << std::endl;
// 				}

// 				// Close the socket and mark as 0 in list for reuse
// 				this->_client_fds[i] = 0;
// 				close(sd);
// 			}
// 		}
// 		std::cerr << "|      ----> LIST ";
// 		for (int i = 0; i < max_sd; i++) 
// 		{
// 			std::cerr <<  this->_client_fds[i] << " |";
// 		}
// 		std::cerr << std::endl << "|END" << std::endl << std::endl << std::endl;
// 	}
// }

// // ------------------- Getter functions -------------------

// int ListenSocket::get_listen_fd(void)
// {
// 	return this->_listen_fd;
// }

// int ListenSocket::get_binder(void)
// {
// 	return this->_binder;
// }

// int ListenSocket::get_listener(void)
// {
// 	return this->_listener;
// }

// int ListenSocket::get_setsockopter(void)
// {
// 	return this->_setsockopter;
// }

// int ListenSocket::get_activity_mon(void)
// {
// 	return this->_activity_mon;
// }

// struct sockaddr_in ListenSocket::get_address(void)
// {
// 	return this->_address;
// }
