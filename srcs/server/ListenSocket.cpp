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

#include "ListenSocket.hpp"

// // ------------------- Constructors -------------------


ListenSocket::ListenSocket(configserv config_serv, int port)
{
	this->_port = port;
	this->_name = config_serv.getName();
	this->_host = config_serv.getHost();
	this->_root = config_serv.getRoot();
	// std::cout << "ListenSocket was called." << std::endl;
}

ListenSocket::ListenSocket( const ListenSocket& copy )
{
	// std::cout << "ListenSocket copy constructor called" << std::endl;
	*this = copy;
}

ListenSocket& ListenSocket::operator=( const ListenSocket& ref )
{
	// std::cout << "ListenSocket assignment operator called" << std::endl;
	if ( this != &ref )
	{
		this->_listen_sd = ref._listen_sd;
		this->_activity_mon = ref._activity_mon;
		this->_address = ref._address;
		this->_name = ref._name;
		this->_port = ref._port;
		this->_host = ref._host;
	}
	return *this;
}

void ListenSocket::initSocket(void)
{
	memset(&this->_address, 0, sizeof(this->_address));
	// Filling Address'struct
	this->_address.sin_family = AF_INET;
	// this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);

	if (inet_pton(AF_INET, this->_host.c_str(), &this->_address.sin_addr) <= 0) {
		perror("Invalid address/ Address not supported");
		exit(EXIT_FAILURE);
	}

	// Establish main server socket
	this->_listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_listen_sd < 0)
	{
		std::cerr << "Issue creating _listen_sd" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set socket to allow multiple connections (reuse address and port)
	int opt = 1;
	if (setsockopt(this->_listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cerr << "Issue setsockopt : " << this->_port << std::endl;
		exit(EXIT_FAILURE);
	}

	// Bind socket to IP address and port
	if (bind(this->_listen_sd, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		std::cerr << "Issue binding _listen_sd at port : " << this->_port << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set a listener ont the socket
	if (listen(this->_listen_sd, 512) < 0)
	{
		std::cerr << "Issue listening _listen_sd at port : " << this->_port << std::endl;
		exit(EXIT_FAILURE);
	}

	// std::cout << "ListenSocket was called." << std::endl;
}

// // ------------------- Getter functions -------------------

int ListenSocket::get_listen_fd(void)
{
	return this->_listen_sd;
}

int ListenSocket::get_activity_mon(void)
{
	return this->_activity_mon;
}

int ListenSocket::get_port(void)
{
	return this->_port;
}

struct sockaddr_in ListenSocket::get_address(void)
{
	return this->_address;
}

std::string ListenSocket::get_root(void)
{
	return this->_root;
}

std::string ListenSocket::get_host(void)
{
	return this->_host;
}

std::string ListenSocket::get_name(void)
{
	return this->_name;
}

void test_connection(int value)
{
	if (value < 0)
	{
		std::cerr << "A connection error occured" << std::endl;
		exit(EXIT_FAILURE);
	}
}
