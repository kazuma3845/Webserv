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

ListenSocket::ListenSocket(configserv config_serv)
{
	std::string port = *config_serv.getListen().begin();
	this->_port = atoi(port.c_str());
	this->_name = config_serv.getName();
	this->_host = config_serv.getHost();
	this->_root = config_serv.getRoot();
	// std::cout << "ListenSocket was called." << std::endl;
}

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

ListenSocket& ListenSocket::operator=( const ListenSocket& ref )
{
	// std::cout << "ListenSocket assignment operator called" << std::endl;
	if ( this != &ref )
	{
		this->_listen_fd = ref._listen_fd;
		this->_activity_mon = ref._activity_mon;
		this->_address = ref._address;
		this->_name = ref._name;
		this->_port = ref._port;
		this->_host = ref._host;
	}
	return *this;
}

// AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY
void ListenSocket::initSocket(void)
{
	memset(&this->_address, 0, sizeof(this->_address));
	// Filling Address'struct
	this->_address.sin_family = AF_INET;
	// this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);
	std::cerr << "IP : " << _host << std::endl;

	if (inet_pton(AF_INET, this->_host.c_str(), &this->_address.sin_addr) <= 0) {
		perror("Invalid address/ Address not supported");
		exit(EXIT_FAILURE);
	}

	// Establish main server socket
	this->_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_listen_fd < 0)
	{
		std::cerr << "Issue creating _listen_fd" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set socket to allow multiple connections (reuse address and port)
	int opt = 1;
	if (setsockopt(this->_listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cerr << "Issue setsockopt : " << this->_port << std::endl;
		exit(EXIT_FAILURE);
	}

	// Bind socket to IP address and port
	if (bind(this->_listen_fd, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		std::cerr << "Issue binding _listen_fd at port : " << this->_port << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set a listener ont the socket
	if (listen(this->_listen_fd, 512) < 0)
	{
		std::cerr << "Issue listening _listen_fd at port : " << this->_port << std::endl;
		exit(EXIT_FAILURE);
	}

	// std::cout << "ListenSocket was called." << std::endl;
}

// // ------------------- Getter functions -------------------

int ListenSocket::get_listen_fd(void)
{
	return this->_listen_fd;
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


void test_connection(int value)
{
	if (value < 0)
	{
		std::cerr << "A connection error occured" << std::endl;
		exit(EXIT_FAILURE);
	}
}
