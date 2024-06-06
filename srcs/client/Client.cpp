#include "Client.hpp"

// ------------------- Constructors -------------------

Client::Client(void)
{
	// std::cout << "New Client called" << std::endl;
}

Client::Client(int fd, ListenSocket &listen_socket)
{
	this->_listen_socket = &listen_socket;
	this->_connected_fd = fd;
	// std::cout << "Client was called." << std::endl;
}

Client::~Client(void)
{
	// std::cout << "Client was destroyed." << std::endl;
}

Client::Client( const Client& copy )
{
	// std::cout << "Client copy constructor called" << std::endl;
	*this = copy;
}

Client& Client::operator=( const Client& ref )
{
	// std::cout << "Client assignment operator called" << std::endl;
	if ( this != &ref )
	{
		this->_connected_fd = ref._connected_fd;
		this->_listen_socket = ref._listen_socket;
	}
	return *this;
}

int Client::get_fd(void)
{
	return this->_connected_fd;
}