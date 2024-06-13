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

void Client::isMethodAllowed()
{
	std::vector<std::string> methods;
	if (this->_request.getCurr_loc().empty()) // si il n'y a pas de location, alors on va chercher les allowed method a la racine
		methods = this->get_listen_socket().get_allow_methods();
	else
		methods = this->_request.getCurr_loc().getAllowMethods();
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); ++it)
	{
		if (this->_request.getMethod() == *it)
			return ;
	}
	throw unauthorizedMethod(405);
}

void Client::redirectInURI() //FIXME: A voir si on veut bien remplacer complètement l'URI
{
	if (this->_request.getCurr_loc().empty())
		return;
	if (!this->_request.getCurr_loc().getReturn().empty())
		this->_request.setURI(this->_request.getCurr_loc().getReturn());
}


const char *Client::unauthorizedMethod::what() const throw()
{
	return ("Unauthorized method requested.");
}