#include "Server.hpp"

// ------------------- Constructors -------------------

Server::Server(void)
{
	std::cout << "New Server called" << std::endl;
}

Server::Server(std::vector<configserv> config_servs) : _max_sd(0)
{
	int nb_ports;
	for (unsigned int i = 0; i < config_servs.size(); ++i)
	{
		nb_ports = (config_servs[i].getListen()).size();
		for (int i_port = 0; i_port != nb_ports; i_port++)
		{
			ListenSocket curr_listen_socket(config_servs[i], config_servs[i].getListen()[i_port]);
			this->_ListenSockets.push_back(curr_listen_socket);
		}
	}
	std::cout << "Server was called." << std::endl;
}

Server::~Server(void)
{
	std::cout << "Server was destroyed." << std::endl;
}

Server::Server(const Server &copy)
{
	std::cout << "Server copy constructor called" << std::endl;
	*this = copy;
}

Server &Server::operator=(const Server &ref)
{
	std::cout << "Server assignment operator called" << std::endl;
	if (this != &ref)
	{
	}
	return *this;
}

void Server::set_server(void)
{
	int current_fd;
	// Clear the socket set to void
	FD_ZERO(&this->_read_sds);
	FD_ZERO(&this->_write_sds);
	for (std::vector<ListenSocket>::iterator it = this->_ListenSockets.begin(); it != this->_ListenSockets.end(); ++it)
	{
		it->initSocket();
		current_fd = it->get_listen_fd();
		// Set new socket as non blocked
		fcntl(current_fd, F_SETFL, O_NONBLOCK);
		// Add every listen sockets to set
		FD_SET(current_fd, &this->_read_sds);
		// Save the max socket descriptor for forther use
		if (current_fd > this->_max_sd)
			this->_max_sd = current_fd;
		std::cerr << "   IP : " << std::setw(14) << std::left << it->get_host() << " port : " << it->get_port() << " fd :" << it->get_listen_fd() << std::endl;
	}
	std::cerr << ">> Max fd : " << this->_max_sd << std::endl;
}

void Server::run_server(void)
{
	fd_set temp_read_sds;
	fd_set temp_write_sds;
	// struct timeval timeout;

	std::cout << std::endl
			  << "##################" << std::endl
			  << "# SERVER STARTED #" << std::endl
			  << "##################" << std::endl;
	while (1)
	{
		temp_read_sds = this->_read_sds;
		temp_write_sds = this->_write_sds;
		// timeout.tv_sec = 1;
		// timeout.tv_usec = 0;

		// Wait for an activity on one of the , select return the value of readies FD
		if (select(this->_max_sd + 1, &temp_read_sds, &temp_write_sds, NULL, NULL) < 0)
			exit(1);
		for (unsigned int i = 0; i < this->_ListenSockets.size(); ++i)
		{
			if (FD_ISSET(this->_ListenSockets[i].get_listen_fd(), &temp_read_sds))
				this->add_client(_ListenSockets[i]);
		}

		// Reading new request
		for (int i = 4; i <= this->_max_sd; ++i)
		{
			// this->_client_sds_map[i].getReq()->printRequest();
			if (_client_sds_map.count(i) && FD_ISSET((this->_client_sds_map[i]).get_fd(), &temp_read_sds))
				read_socket(this->_client_sds_map[i]);
			// std::cerr << "####### Client : "  << this->_client_sds_map[i].getReq()->getClient()->get_fd() << std::endl;
		}
		// Writing request response
		for (int i = 4; i <= this->_max_sd; ++i)
		{
			if (_client_sds_map.count(i) && FD_ISSET((this->_client_sds_map[i]).get_fd(), &temp_write_sds))
				write_socket(this->_client_sds_map[i]);
		}
		check_timeout();
	}
	std::cerr << "SERVER END " << std::endl
			  << std::endl;
}

void Server::add_client(ListenSocket &listen_socket)
{
	struct sockaddr_in address;
	int addrlen;
	int new_socket;

	addrlen = sizeof(address);
	new_socket = accept(listen_socket.get_listen_fd(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (new_socket == -1)
	{
		std::cerr << "Issue accepting new_socket :" << new_socket
				  << " " << listen_socket.get_listen_fd() << std::endl;
		exit(1);
	}

	// Set new socket as non blocked
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Issue setting O_NONBLOCK" << std::endl;
		exit(1);
	}
	Client new_client(new_socket, listen_socket);
	// ------------- Message for testing
	std::cout << std::endl
			  << "|" << std::endl
			  << "|   New connection: socket fd is " << new_socket << " | "
			  << inet_ntoa(address.sin_addr) << " | " << ntohs(address.sin_port) << std::endl
			  << "|" << std::endl;

	// Add new socket to fd_set
	FD_SET(new_socket, &this->_read_sds);
	if (new_socket > this->_max_sd)
		this->_max_sd = new_socket;
	if (_client_sds_map.count(new_socket) != 0)
		_client_sds_map.erase(new_socket);
	this->_client_sds_map[new_socket] = new_client;
}

void Server::read_socket(Client &client)
{
	int socket = client.get_fd();
	Redirection redirect;
	// Response response;

	client.setTimeout();

	try
	{
		if (client.getFlag() == PARSING_REQUEST)
			client.getReq()->parseRequest(socket);
	
		if (client.getFlag() == EXPECTING)
		{
			FD_CLR(socket, &this->_read_sds);
			FD_SET(socket, &this->_write_sds);
		}
		if (client.getFlag() == REDIRECTING)
		{
			client.getReq()->checkRequest();
			redirect.path(*client.getReq(), *client.getResponse());
		}
		if (client.getFlag() == HANDLING_REQUEST)
			client.getHandler()->start();
	
		if (client.getFlag() == WRITING_RESPONSE)
		{
			client.getResponse()->setHTTPVersion(client.getReq()->getHttpVersion());
			client.getResponse()->setConnectionType(client.getKeepAlive());
			if (client.getReq()->getHasReturn())
				client.getResponse()->setStatusCode(301);
			client.getResponse()->formatResponse(client, *client.getReq());
			client.setResp(client.getResponse()->getResp());
			FD_CLR(socket, &this->_read_sds);
			FD_SET(socket, &this->_write_sds);
		}
	}
	catch (const ErrorWebServ &e)
	{
		// std::cerr << "####### Client :"  << std::endl;
		std::cerr << "Error number: " << e.getErrorCode() << std::endl;
		std::cerr << "What happened : " << e.what() << std::endl;
		client.setKeepAlive(false);
		client.getResponse()->setStatusCode(e.getErrorCode());
		client.getResponse()->setStatusMessage(e.what());
		std::stringstream ss;
		ss << e.getErrorCode();
		if (client.get_listen_socket().get_error().compare(ss.str()) == 0)
			client.getResponse()->ErrorBody(e.getErrorCode(), client, true);
		else
			client.getResponse()->ErrorBody(e.getErrorCode(), client, false);
		client.getResponse()->setConnectionType(false);
		client.getResponse()->setContentType("text/html");
		client.getResponse()->formatResponse(client, *client.getReq());
		client.setResp(client.getResponse()->getResp());
		FD_CLR(socket, &this->_read_sds);
		FD_SET(socket, &this->_write_sds);
	}
}

void Server::write_socket(Client &client)
{
	int socket = client.get_fd();

	write(socket, client.getResp().c_str(), client.getResp().length());
	if (client.getFlag() == EXPECTING)
	{
		client.setResp(""); // NULL segfault
		client.setFlag(PARSING_REQUEST);
		client.getReq()->setStatus(PARSING_BODY);
		FD_CLR(socket, &this->_write_sds);
		FD_SET(socket, &this->_read_sds);
		return;
	}
	//-----------------------------------------------------------------------
	// // Only to show the request content; PRINT REPONSE
	// std::istringstream contentStream(client.getResp());
	// std::string line;
	// // std::cerr << std::endl << "|" << std::endl << "|   CONTENT WRITTEN -> " << socket << std::endl;
	// while (std::getline(contentStream, line))
	// {
	// 	std::cerr << line << std::endl;
	// }
	//-----------------------------------------------------------------------

	// Remove socket from read to write
	FD_CLR(socket, &this->_write_sds);
	if (client.getKeepAlive()) // status is herited from Request parsing
	{
		FD_SET(socket, &this->_read_sds);
		client.reUseClient();
	}
	else
	{
		std::cerr << "End of connection from client fd : " << socket << std::endl;
		close(socket);
		this->_client_sds_map.erase(socket);
	}
}

void Server::check_timeout(void)
{
	for (int i = 0; i <= this->_max_sd; ++i)
	{
		if (_client_sds_map.count(i) && (time(NULL) - _client_sds_map[i].get_connected_time() > TIMEOUT_LIMIT))
		{
			std::cerr << "Disconnection from client fd : " << i << " sec asgo " << time(NULL) - _client_sds_map[i].get_connected_time() << std::endl;
			if (FD_ISSET(i, &_read_sds))
				FD_CLR(i, &_read_sds);
			if (FD_ISSET(i, &_write_sds))
				FD_CLR(i, &_write_sds);
			if (i >= _max_sd)
				_max_sd--;
			close(i);
			_client_sds_map.erase(i);
		}
	}
}
