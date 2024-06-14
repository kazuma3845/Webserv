#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <IP_ADDRESS> <PORT>" << std::endl;
		return -1;
	}

	std::string ip_address = argv[1];
	int port = std::stoi(argv[2]);

	int sockfd;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];
	std::string request = "GET /cgi HTTP/1.1\r\nHost: " + ip_address + "\r\nConnection: close\r\n\r\n";

	// Create socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "Socket creation error" << std::endl;
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address/ Address not supported" << std::endl;
		return -1;
	}

	// Connect to server
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Connection failed" << std::endl;
		return -1;
	}

	// Send HTTP GET request
	send(sockfd, request.c_str(), request.length(), 0);
	std::cout << "HTTP GET request sent" << std::endl;

	// Read the response
	int bytes_read;
	while ((bytes_read = read(sockfd, buffer, BUFFER_SIZE - 1)) > 0) {
		buffer[bytes_read] = '\0'; // Null-terminate the buffer
		std::cout << buffer;
	}

	// Close the socket
	close(sockfd);

	return 0;
}
