#include "Client.hpp"

// ------------------- Constructors -------------------

Client::Client(int fd, ListenSocket &listen_socket)
{
	this->_listen_socket = listen_socket;
	this->_connected_sd = fd;
	this->_connected_time = time(NULL);
	this->_request = new Request(this);
	this->_flag = PARSING_REQUEST;
	// std::cout << "Client was called. " << std::endl;
}
Client::Client() {

}

Client::~Client(void)
{
	delete _request;
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
		this->_connected_time = ref._connected_time;
		this->_flag = ref._flag;
		this->_request = new Request(*ref._request);
		_request->setClient(this);
	}
	return *this;
}

void Client::reUseClient(void)
{
	_rep.clear();
	delete _request;
	_request = new Request(this);
	this->_flag = PARSING_REQUEST;
}

int Client::get_fd(void)
{
	return this->_connected_sd;
}

ListenSocket Client::get_listen_socket(void)
{
	return this->_listen_socket;
}

bool Client::getKeepAlive()
{
	return (_keepAlive);
}

Request *Client::getReq(void)
{
	return this->_request;
}

void Client::setResp(std::string rep)
{
	_rep = rep;
}

void Client::setReq(Request *request)
{
	this->_request = request;
}

std::string Client::getResp() const
{
	return _rep;
}

time_t Client::get_connected_time(void)
{
	return _connected_time;
}

Flag Client::getFlag()
{
	return _flag;
}

void Client::setHeaders(const std::map<std::string, std::string>& headers)
{
	_headers = headers;
}

void Client::setKeepAlive(bool status)
{
	_keepAlive = status;
}

void	Client::setTimeout(void)
{
	_connected_time = time(NULL);
}

void Client::setFlag(Flag flag)
{
	this->_flag = flag;
}
