/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasoni <mmasoni@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 14:49:15 by mmasoni           #+#    #+#             */
/*   Updated: 2024/04/25 13:06:48 by mmasoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListenSocket.hpp"

// ------------------- Constructors -------------------

ListenSocket::ListenSocket(void)
{
	std::cout << "New ListenSocket called" << std::endl;
}

ListenSocket::ListenSocket(configserv config_serv)
{
	std::string port = *config_serv.getListen().begin();
	this->_port = atoi(port.c_str());
	this->_name = config_serv.getName();
	this->_host = config_serv.getHost();
	this->_root = config_serv.getRoot();
	std::cout << "ListenSocket was called." << std::endl;

}

ListenSocket::~ListenSocket(void)
{
	std::cout << "ListenSocket was destroyed." << std::endl;
}

ListenSocket::ListenSocket( const ListenSocket& copy )
{
	std::cout << "ListenSocket copy constructor called" << std::endl;
	*this = copy;
}

ListenSocket& ListenSocket::operator=( const ListenSocket& ref )
{
	std::cout << "ListenSocket assignment operator called" << std::endl;
	if ( this != &ref )
	{
		this->_listen_fd = ref._listen_fd;
		this->_binder = ref._binder;
		this->_listener = ref._listener;
		this->_setsockopter = ref._setsockopter;
		this->_activity_mon = ref._activity_mon;
		this->_address = ref._address;
		this->_name = ref._name;
	}
	return *this;
}

// AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY
void ListenSocket::initSocket(void)
{
	memset(&this->_address, 0, sizeof(this->_address));
	// Filling Address'struct
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);

	// Establish main server socket
	this->_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	test_connection(this->_listen_fd);

	// Set socket to allow multiple connections (reuse address and port)
	int opt = 1;
	this->_setsockopter = setsockopt(this->_listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	test_connection(this->_setsockopter);

	// Bind socket to IP address and port
	this->_binder = bind(this->_listen_fd, (struct sockaddr *)&this->_address, sizeof(this->_address));
	test_connection(this->_binder);

	// Set a listener ont the socket
	this->_listener = listen(this->_listen_fd, 512);
	test_connection(this->_listener);

	std::cout << "ListenSocket was called." << std::endl;
}

// ------------------- Getter functions -------------------

int ListenSocket::get_listen_fd(void)
{
	return this->_listen_fd;
}

int ListenSocket::get_binder(void)
{
	return this->_binder;
}

int ListenSocket::get_listener(void)
{
	return this->_listener;
}

int ListenSocket::get_setsockopter(void)
{
	return this->_setsockopter;
}

int ListenSocket::get_activity_mon(void)
{
	return this->_activity_mon;
}

struct sockaddr_in ListenSocket::get_address(void)
{
	return this->_address;
}


void test_connection(int value)
{
	if (value < 0)
	{
		std::cerr << "A connection error occured" << std::endl;
		exit(EXIT_FAILURE);
	}
}
