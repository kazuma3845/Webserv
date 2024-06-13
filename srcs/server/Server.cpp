#include "Server.hpp"

// ------------------- Constructors -------------------

Server::Server(void)
{
	std::cout << "New Server called" << std::endl;
}

Server::Server(std::vector<configserv>	config_servs) : _max_sd(0)
{
	int nb_ports;

	for (std::vector<configserv>::iterator it = config_servs.begin(); it != config_servs.end(); ++it)
	{
		nb_ports = (it->getListen()).size();
		for (int i_port = 0; i_port != nb_ports; i_port++)
		{
			ListenSocket curr_listen_socket = ListenSocket(*it, it->getListen()[i_port]);
			this->_ListenSockets.push_back(curr_listen_socket);
		}
	}
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
		
	}
	return *this;
}

void Server::set_server(void)
{
	int	current_fd;
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
		std::cerr <<"   IP : " << std::setw(14) << std::left << it->get_host() << " port : " << it->get_port() << " fd :" << it->get_listen_fd() << std::endl;
	}
	std::cerr << ">> Max fd : " << this->_max_sd << std::endl;
}

void Server::run_server(void)
{
	fd_set		temp_read_sds;
	fd_set		temp_write_sds;

	std::cout << std::endl << "##################" << std::endl << "# SERVER STARTED #" << std::endl << "##################" << std::endl;
	while (1) {
		temp_read_sds = this->_read_sds;
		temp_write_sds = this->_write_sds;
		std::cerr << std::endl << "| START LOOP: " << std::endl;

		// Wait for an activity on one of the , select return the value of readies FD
		if (select(this->_max_sd + 1, &temp_read_sds, NULL, NULL, NULL) <= 0)
			exit(1);
		for(std::vector<ListenSocket>::iterator it = this->_ListenSockets.begin(); it != this->_ListenSockets.end(); ++it)
		{
			if (FD_ISSET(it->get_listen_fd(), &temp_read_sds))
				this->add_client(*it);
		}
		
		// Reading new request
		for(int i = 0; i <= this->_max_sd; ++i)
		{
			if (_client_sds_map.count(i) && FD_ISSET((this->_client_sds_map[i]).get_fd(), &temp_read_sds))
			{
				std::cerr << "------>> ENTRE" << std::endl;
				read_socket(this->_client_sds_map[i]);
				// this->_client_sds_map[i].get_request()->printRequest();
			}
		}
		// Writing request response
		for(int i = 0; i <= this->_max_sd; ++i)
		{
			if (_client_sds_map.count(i) && FD_ISSET((this->_client_sds_map[i]).get_fd(), &_write_sds))
				write_socket(this->_client_sds_map[i]);
		}
		std::cerr << "| END LOOP" << std::endl;
	}
	std::cerr << "SERVER END " << std::endl << std::endl;
}

void Server::add_client(ListenSocket &listen_socket)
{
	struct sockaddr_in	address;
	int					addrlen;
	int					new_socket;

	addrlen = sizeof(address);
	new_socket = accept(listen_socket.get_listen_fd(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
	if (new_socket == -1)
	{
		std::cerr << "Issue accepting new_socket :" << new_socket 
		<< " " << listen_socket.get_listen_fd()
		<< " " << strerror(errno)
		<< std::endl;
		exit(1);
	}

	// Set new socket as non blocked
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Issue setting O_NONBLOCK" << std::endl;
		exit(1);
	}
	Client new_client = Client(new_socket, listen_socket);
	// ------------- Message for testing
	std::cout << "|" << std::endl << "|   New connection: socket fd is " << new_socket << " | " << inet_ntoa(listen_socket.get_address().sin_addr) << " | "<< ntohs(listen_socket.get_address().sin_port) << std::endl << "|" << std::endl;

	// Add new socket to fd_set
	FD_SET(new_socket, &this->_read_sds);
	if (new_socket > this->_max_sd)
			this->_max_sd = new_socket;
	this->_client_sds_map.insert(std::make_pair(new_socket, new_client));
	// this->_Clients.push_back(new_client);
}

void Server::read_socket(Client& client)
{
	char			buffer[MESSAGE_BUFFER];
	int				socket = client.get_fd();

	memset(buffer, 0, sizeof(buffer));
	int has_content = read(socket, buffer, MESSAGE_BUFFER);
	if (has_content)
	{
		Request req(client);
		req.parseRequest(buffer);
		//TODO: client.set_request(req);
		req.printRequest();
		// Only to show the request content;
		// std::istringstream contentStream(buffer);
		// std::string line;

		// std::cerr << "|" << std::endl << "|   CONTENT READED ->" << std::endl;
		// while (std::getline(contentStream, line)) {
		// 	std::cerr << "|      " << line << std::endl;
		// }
		// client.get_request()->printRequest();
	}
	// client.get_request()->printRequest();
	// Remove socket from read to write
	FD_CLR(socket, &this->_read_sds);
	FD_SET(socket, &this->_write_sds);
	// close(socket);
	// this->_client_sds_map.erase(socket);
}

void Server::write_socket(Client &client)
{
	int				socket = client.get_fd();
	Redirection rep;
	Request request(client);
	try
	{
	//FIXME:------------------------------------------------------
	//			APPELLE FONCTION DE FRANCOIS
	//------------------------------------------------------
		//TODO: rep.callPath(request);
		rep.reponseCGI(request);
	}
	catch(const std::exception &e)
	{
		std::cerr << "Error number: " << e.what() << std::endl;
		// rep.reponseError();
	}
	// Envoyer la réponse HTTP complète
	write(socket, rep.getRep().c_str(), rep.getRep().length());
	// write(socket, HTML_CONTENT, strlen(HTML_CONTENT));

	//-----------------------------------------------------------------------
	// Only to show the request content; PRINT REPONSE
	std::istringstream contentStream(rep.getRep());
	std::string line;

	std::cerr << "|" << std::endl << "|   CONTENT WRITTEN ->" << std::endl;
	while (std::getline(contentStream, line)) {
		std::cerr << "|      " << line << std::endl;
	}
	//-----------------------------------------------------------------------

	// Remove socket from read to write
	FD_CLR(socket, &this->_write_sds);

	// FD_SET(sd, &this->_write_sds);
	close(socket);
	this->_client_sds_map.erase(socket);
}
