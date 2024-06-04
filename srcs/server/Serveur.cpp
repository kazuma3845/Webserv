/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasoni <mmasoni@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 14:49:15 by mmasoni           #+#    #+#             */
/*   Updated: 2024/06/04 10:48:15 by mmasoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// ------------------- Constructors -------------------

Server::Server(void)
{
	std::cout << "New Server called" << std::endl;
}

Server::Server(std::vector<configserv>	_configservs)
{
	std::vector<ListenSocket>	listen_sockets;
	

	this->_listen_sockets = listen_sockets;
	std::cout << "Server was called." << std::endl;

}

Server::~Server(void)
{
	std::cout << "Server was destroyed." << std::endl;
}

Server::Server( const Server& copy )
{
	std::cout << "Server copy constructor called" << std::endl;
	*this = copy;
}

Server& Server::operator=( const Server& ref )
{
	std::cout << "Server assignment operator called" << std::endl;
	if ( this != &ref )
	{
		this->_ListenSockets = ref._ListenSockets;
	}
	return *this;
}

void Server::test_connection(int value)
{
	if (value < 0)
	{
		std::cerr << "A connection error occured" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::run_server(void)
{
	fd_set			read_fds;
	int				sd; /* Socket descriptor */
	int				max_sd; /* Max Socket descriptor */
	int				new_socket;
	int				non_block;
	int				addrlen;
	char			buffer[MESSAGE_BUFFER];

	std::cout  << std::endl << "SERVER STARTED" << std::endl;
	while (1) {
		std::cerr << "|START" << std::endl;
		// Clear the socket set to void
		FD_ZERO(&read_fds);

		// Add master socket to set
		FD_SET(this->_listen_fd, &read_fds);

		// Save the max socket descriptor for forther use
		max_sd = this->_listen_fd;

		// Add child sockets to set
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (sd == 0)
				continue ;
			sd = this->_client_fds[i];
			FD_SET(sd, &read_fds);  /* If valid socket descriptor then add to read list */
			if (sd > 0) {
				FD_SET(sd, &read_fds);
			}
			if (sd > max_sd) /* Highest file descriptor number for select function */
				max_sd = sd;
		}

		// Wait for an activity on one of the , select return the value of readies FD
		this->_activity_mon = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
		test_connection(this->_activity_mon);

		// If something happened on the master socket, it's an incoming connection
		if (FD_ISSET(this->_listen_fd, &read_fds))
		{
			addrlen = sizeof(this->_address);
			new_socket = accept(this->_listen_fd, (struct sockaddr *)&this->_address, (socklen_t*)&addrlen);
			test_connection(new_socket);

			// Set new socket as non blocked
			non_block = fcntl(new_socket, F_SETFL, O_NONBLOCK);
			test_connection(non_block);

			// ------------- Message for testing
			std::cout << "|   New connection: socket fd is " << new_socket << " | " << inet_ntoa(this->_address.sin_addr) << " | "<< ntohs(this->_address.sin_port) << std::endl;

			// Add new socket to array of sockets
			for (int i = 0; i < MAX_CLIENTS; i++) 
			{
				if (this->_client_fds[i] == 0)
				{
					std::cerr << "|       new_socket added : "<< new_socket << std::endl;
					FD_SET(new_socket, &read_fds);
					this->_client_fds[i] = new_socket;
					break;
				}
			}
		}

		// Else it's some IO operation on some other socket and we check every sd
		for (int i = 0; i < MAX_CLIENTS; i++) {
			sd = this->_client_fds[i];
			if (FD_ISSET(sd, &read_fds)) {
				std::cerr << "|      ----> Socket " << sd << " was readed and will close" << std::endl;

				// Handle the stuff to do late .....
				memset(buffer, 0, sizeof(buffer));
				int has_content = read(sd, buffer, MESSAGE_BUFFER);
				if (has_content)
				{
					std::cerr << "-------------- CONTENT --------------" << std::endl;
					std::cerr << buffer << std::endl;
					std::cerr << "-------------- ------- --------------" << std::endl;
				}

				// Close the socket and mark as 0 in list for reuse
				this->_client_fds[i] = 0;
				close(sd);
			}
		}
		std::cerr << "|      ----> LIST ";
		for (int i = 0; i < max_sd; i++) 
		{
			std::cerr <<  this->_client_fds[i] << " |";
		}
		std::cerr << std::endl << "|END" << std::endl << std::endl << std::endl;
	}
}

// ------------------- Getter functions -------------------

int Server::get_listen_fd(void)
{
	return this->_listen_fd;
}

int Server::get_binder(void)
{
	return this->_binder;
}

int Server::get_listener(void)
{
	return this->_listener;
}

int Server::get_setsockopter(void)
{
	return this->_setsockopter;
}

int Server::get_activity_mon(void)
{
	return this->_activity_mon;
}

struct sockaddr_in Server::get_address(void)
{
	return this->_address;
}
