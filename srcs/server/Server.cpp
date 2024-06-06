#include "Server.hpp"

// ------------------- Constructors -------------------

Server::Server(void)
{
	std::cout << "New Server called" << std::endl;
}

Server::Server(std::vector<configserv>	config_servs) : _max_sd(0)
{
	for (std::vector<configserv>::iterator it = config_servs.begin(); it != config_servs.end(); ++it)
	{
		ListenSocket curr_listen_socket = ListenSocket(*it);
		this->_listen_sockets.push_back(curr_listen_socket);
		std::cerr << "=====>> Creating ListenSocket : " << curr_listen_socket.get_port() << std::endl;
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

void Server::test_connection(int value)
{
	if (value < 0)
	{
		std::cerr << "A connection error occured" << std::endl;
		exit(1);
	}
}

void Server::set_server(void)
{
	int	current_fd;
	// Clear the socket set to void
	FD_ZERO(&this->_read_sds);
	for (std::vector<ListenSocket>::iterator it = this->_listen_sockets.begin(); it != this->_listen_sockets.end(); ++it)
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
		std::cerr << "=====>> TEST : port : " << it->get_port() << " fd :" << it->get_listen_fd() << std::endl;
	}
	std::cerr << "=====>> TEST : max : " << this->_max_sd << std::endl;
}

void Server::run_server(void)
{
	fd_set		temp_read_sds;

	std::cout  << std::endl << "SERVER STARTED" << std::endl;
	while (1) {
		temp_read_sds = this->_read_sds;
		std::cerr << std::endl << "|START : " << std::endl;

		// Wait for an activity on one of the , select return the value of readies FD
		if (select(this->_max_sd + 1, &temp_read_sds, NULL, NULL, NULL) <= 0)
			exit(1);
		for(std::vector<ListenSocket>::iterator it = this->_listen_sockets.begin(); it != this->_listen_sockets.end(); ++it)
		{
			if (FD_ISSET(it->get_listen_fd(), &temp_read_sds))
				this->add_client(*it);
		}
		
		// If something happened on the client socket
		for(int i = 0; i <= this->_max_sd; ++i)
		{
			if (_client_sd_map.count(i) && FD_ISSET((this->_client_sd_map[i]).get_fd(), &temp_read_sds))
				read_socket(this->_client_sd_map[i]);
		}
		//================================
		//================================
		// =======> faire pour write
		//================================
		//================================
	}
	std::cerr << "SERVER END " << std::endl;
}

void Server::add_client(ListenSocket listen_socket)
{
	struct sockaddr_in	address;
	int					addrlen;
	int					new_socket;

	addrlen = sizeof(address);
	std::cout << "listen_socket.get_listen_fd() " << listen_socket.get_listen_fd() << std::endl;
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
	std::cout << "|   New connection: socket fd is " << new_socket << " | " << inet_ntoa(listen_socket.get_address().sin_addr) << " | "<< ntohs(listen_socket.get_address().sin_port) << std::endl;

	// Add new socket to vector of client
	FD_SET(new_socket, &this->_read_sds);
	if (new_socket > this->_max_sd)
			this->_max_sd = new_socket;
	this->_client_sd_map.insert(std::make_pair(new_socket, new_client));
	// this->_clients.push_back(new_client);
}

void Server::read_socket(Client client)
{
	char			buffer[MESSAGE_BUFFER];
	int				socket = client.get_fd();

	std::cerr << "|      ----> Socket " << socket << " was readed and will close" << std::endl;

	// Handle the stuff to do late .....
	memset(buffer, 0, sizeof(buffer));
	int has_content = read(socket, buffer, MESSAGE_BUFFER);
	if (has_content)
	{
		client.set_request_content(buffer);
		std::cerr << "-------------- CONTENT --------------" << std::endl;
		std::cerr << client.get_request_content() << std::endl;
		std::cerr << "-------------- ------- --------------" << std::endl;
	}

	// Remove socket from read to write
	FD_CLR(socket, &this->_read_sds);
	// FD_SET(sd, &this->_write_sds);
	close(socket);
	this->_client_sd_map.erase(socket);
}
