#include "Client.hpp"

// ------------------- Constructors -------------------

Client::Client(int fd, ListenSocket &listen_socket)
{
	this->_listen_socket = listen_socket;
	this->_connected_sd = fd;
	// std::cout << "Client was called." << std::endl;
}
Client::Client() {

}

Client::~Client(void)
{
	// std::cout << "Client was destroyed." << std::endl;
}

// Client::Client( const Client& copy )
// {
// 	// std::cout << "Client copy constructor called" << std::endl;
// 	*this = copy;
// }

Client& Client::operator=( const Client& ref )
{
	// std::cout << "Client assignment operator called" << std::endl;
	if ( this != &ref )
	{
		this->_connected_sd = ref._connected_sd;
		this->_listen_socket = ref._listen_socket;
		// this->_request = ref._request;
	}
	return *this;
}

int Client::get_fd(void)
{
	return this->_connected_sd;
}

// std::string Client::get_request_content(void)
// {
// 	return this->_request_content;
// }

ListenSocket Client::get_listen_socket(void)
{
	return this->_listen_socket;
}

void Client::setResp(std::string rep)
{
	_rep = rep;
}

std::string Client::getResp() const
{
	return _rep;
}
