#include "Client.hpp"
#include "../response/response.hpp"
#include "../request_handler/Handler.hpp"

// ------------------- Constructors -------------------

Client::Client(int fd, ListenSocket &listen_socket)
{
	this->_listen_socket = listen_socket;
	this->_connected_sd = fd;
	this->_connected_time = time(NULL);
	this->_request = new Request(this);
	this->_response = new Response();
	this->_handler = new Handler(*_request, *_response);
	this->_flag = PARSING_REQUEST;
}
Client::Client()
{
}

Client::~Client(void)
{
	delete _request;
}

Client &Client::operator=(const Client &ref)
{
	if (this != &ref)
	{
		delete _handler;
		delete _response;
		delete _request;

		_connected_sd = ref._connected_sd;
		_listen_socket = ref._listen_socket;
		_connected_time = ref._connected_time;
		_flag = ref._flag;

		_request = new Request(*ref._request);
		_response = new Response(*ref._response);
		_handler = new Handler(*_request, *_response);

		_request->setClient(this); //! important pour set up le client de la requete a cette nouvelle instance
	}
	return *this;
}

void Client::reUseClient(void)
{
	_rep.clear();
	delete _request;
	_request = new Request(this);

	delete _response;
	_response = new Response();

	delete _handler;
	_handler = new Handler(*_request, *_response);

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
Response *Client::getResponse()
{
	return (this->_response);
}
Handler *Client::getHandler()
{
	return (this->_handler);
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

void Client::setHeaders(const std::map<std::string, std::string> &headers)
{
	_headers = headers;
}

void Client::setKeepAlive(bool status)
{
	_keepAlive = status;
}

void Client::setTimeout(void)
{
	_connected_time = time(NULL);
}

void Client::setFlag(Flag flag)
{
	this->_flag = flag;
}
