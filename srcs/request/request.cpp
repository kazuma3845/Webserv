#include "request.hpp"

// ------------------------------------ COPLIEN -- //

Request::Request()
{
	// std::cout << "Request instance created." << std::endl;
}

Request::Request(Client *client) : _hasReturn(false), client(client)
{
	// std::cout << "Request instance with pointer on client created." << std::endl;
}

Request::~Request()
{
	// std::cout << "Request instance destroyed ..." << std::endl;
}
// ---------------------------------- GETTERS -- //

std::string Request::getMethod()
{
	return (_method);
}

std::string Request::getURI()
{
	return (_uri);
}

std::string Request::getHttpVersion()
{
	return (_httpVersion);
}

std::string Request::getBody()
{
	return (_body);
}

std::map<std::string, std::string> Request::getHeaders()
{
	return (_headers);
}

location Request::getCurr_loc()
{
	return _curr_loc;
}

std::string Request::getFilePath()
{
	return (_file_path);
}

std::string Request::getFullPath()
{
	return (_full_path);
}

std::string Request::getQueryString()
{
	return (_queryString);
}
Client *Request::getClient()
{
	return client;
}

bool Request::getHasReturn()
{
	return _hasReturn;
}

// ---------------------------------- SETTERS -- //

void Request::setURI(std::string uri)
{
	_uri = uri;
}
void Request::setLocation(location &loc)
{
	_curr_loc = loc;
}

void Request::setFilePath(std::string filePath)
{
	_file_path = filePath;
}

void Request::setFullPath(std::string fullPath)
{
	_full_path = fullPath;
}

void Request::setQueryString(std::string queryString)
{
	_queryString = queryString;
}

void Request::setClient(Client *client)
{
	this->client = client;
}
// ---------------------------------- OTHER FUNCTIONS -- //

// * This function prints out the parsed contents of an HTTP request, including method, URI, headers, body, and other details.

void Request::printRequest()
{
	std::cout << " -- REQUEST PARSED -- " << std::endl;
	std::cout << std::endl
			  << "Method : " << _method << std::endl;
	std::cout << "URI : " << _uri << std::endl;
	std::cout << "Version : " << _httpVersion << std::endl;

	// Iterate through headers and print each key-value pair
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
		std::cout << it->first << " : " << it->second << std::endl;

	// Print body or indicate if it's empty
	// if (!_body.empty())
	//     std::cout << "Body : " << _body << std::endl;
	// else
	//     std::cout << "! Body is EMPTY" << std::endl;

	// Print location or indicate if it's empty
	if (!_curr_loc.empty())
		std::cout << "Location : " << _curr_loc.getName() << std::endl;
	else
		std::cout << "! Location is EMPTY" << std::endl;

	std::cout << "File_path : " << _file_path << std::endl;
	std::cout << "Full_path : " << _full_path << std::endl;

	// Print query string or indicate if none found
	if (!_queryString.empty())
		std::cout << "Query String : " << _queryString << std::endl;
	else
		std::cout << "! No query string found." << std::endl;

	std::cout << std::endl;
	std::cout << " -- END OF PARSED REQUEST -- " << std::endl;
}

void Request::extractQueryString()
{
	size_t pos = _uri.find('?');

	std::string tmp = _uri.substr(0, pos);
	_queryString = _uri.substr(pos + 1);

	_uri = tmp;
}

// * Parses the request line of an HTTP request and validates its components.
// * Extracts the method, URI, and HTTP version from the provided line.
// * Validates the extracted components and throws exceptions for invalid inputs.

void Request::parseRequestLine(std::string line)
{
	// Create a string stream from the request line
	std::istringstream ss(line);

	// Extract method, URI, and HTTP version from the string stream
	ss >> _method >> _uri >> _httpVersion;

	// If URI contains a query string, extract it
	if (_uri.find('?') != std::string::npos)
		extractQueryString();

	char extra;
	// Validate the extracted components and ensure no extra characters in the stream
	if (_method.empty() || _uri.empty() || _uri.front() != '/' || _httpVersion.empty() || _httpVersion.substr(0, 5) != "HTTP/" || ss >> extra)
		throw wrongRLInput(400); // Throw exception for invalid request line input

	// Ensure the HTTP version is supported
	if (_httpVersion != "HTTP/1.1")
		throw unsupportedHTTPVersion(505); // Throw exception for unsupported HTTP version
}

// * Parses a single header line and stores the key-value pair in the headers map.
// * Throws an exception if the header line format is invalid.

void Request::parseHeaders(std::string line)
{
	// Create a string stream from the header line
	std::istringstream ss(line);
	std::string key, value;

	// Extract key and value from the string stream, separated by ':'
	if (std::getline(ss, key, ':') && std::getline(ss, value))
	{
		// Trim leading whitespace from value
		size_t start = value.find_first_not_of(" \t\r\n");
		if (start != std::string::npos)
			value.erase(0, start);

		// Trim trailing whitespace from value
		size_t end = value.find_last_not_of(" \t\r\n");
		if (end != std::string::npos)
			value.erase(end + 1);

		// Store the key-value pair in the headers map
		_headers[key] = value;
	}
	else
		throw headerParsingError(400); // Throw exception for invalid header format
}

// * Parses a chunked HTTP body received on the given file descriptor (fd),
// * appending it to the internal body buffer of the Request object.

void Request::processChunkedBody(int fd)
{
	char buffer[1024];
	ssize_t bytesRead = 0;
	std::string chunkSizeLine;
	size_t chunkSize = 0;
	size_t maxSize = client->get_listen_socket().get_clientSize(); // Example max size
	size_t totalSize = 0;

	std::cout << "Starting to read chunked body ..." << std::endl;
	while (true)
	{
		// Read until \r\n is found (start of chunk size line)
		do
		{
			bytesRead = read(fd, buffer, 1);
			if (bytesRead < 1)
			{
				throw std::runtime_error("Failed to read from socket");
			}
			chunkSizeLine += buffer[0];
		} while (chunkSizeLine.size() < 2 || chunkSizeLine.substr(chunkSizeLine.size() - 2) != "\r\n");

		chunkSizeLine = chunkSizeLine.substr(0, chunkSizeLine.size() - 2); // Remove \r\n
		std::stringstream ss(chunkSizeLine);
		ss >> std::hex >> chunkSize;
		std::cout << "ChunkSize is :" << chunkSize << std::endl;

		if (chunkSize == 0)
			break;

		if (totalSize + chunkSize > maxSize)
		{
			throw std::runtime_error("Payload too large");
		}

		std::vector<char> chunkData(chunkSize);
		size_t totalRead = 0;

		while (totalRead < chunkSize)
		{
			bytesRead = read(fd, &chunkData[totalRead], chunkSize - totalRead);
			if (bytesRead < 1)
			{
				throw std::runtime_error("Failed to read chunk data");
			}
			totalRead += bytesRead;
		}

		_body.append(chunkData.begin(), chunkData.end());
		read(fd, buffer, 2); // Read trailing \r\n
		chunkSizeLine.clear();
	}
};

// * Parses the body of an HTTP request from a given file descriptor.
void Request::parseBody(int fd, unsigned int length)
{
	if (client->get_listen_socket().get_clientSize() < length) // Check if client request size exceeds allowed limit
		throw bodySize(413);

	char *buffer = new char[length + 1]; // Allocate buffer for reading body content
	size_t totalBytesRead = 0;
	ssize_t bytesRead = 0;

	while (totalBytesRead < length) // Loop until entire body length is read
	{
		bytesRead = read(fd, buffer + totalBytesRead, length - totalBytesRead);
		if (bytesRead < 0) // Check for read error
		{
			delete[] buffer;
			throw std::runtime_error("Error reading file");
		}
		else if (bytesRead == 0) // Check if client closed connection prematurely
		{
			delete[] buffer;
			throw std::runtime_error("Connection closed by client");
		}
		totalBytesRead += bytesRead; // Update total bytes read counter
	}
	if (totalBytesRead != length)
		throw shorterBodyContent(400);
	buffer[totalBytesRead] = '\0';
	_body.assign(buffer, totalBytesRead);
	delete[] buffer;

	std::cout << "Length expected: " << length << ", Bytes read: " << totalBytesRead << std::endl;
}

// * This function checks various aspects of an HTTP request to ensure it meets certain criteria.
// * It parses the URI, checks if the method is allowed, handles redirections, checks file existence,
// * and sets the connection mode based on headers.

void Request::checkRequest()
{
	parseUri(); // Parses the URI to get the location of the requested resource.

	isMethodAllowed(); // Checks if the HTTP method used in the request is allowed for the resource.

	redirectInURI(); // Checks if there is a redirection specified for the URI.

	checkFile(F_OK); // Checks if the requested file exists and can be accessed.

	// Sets up the connection mode in the Client object to keep-alive if the "Connection" header is set to "keep-alive".
	if (_headers["Connection"] == "keep-alive")
		client->setKeepAlive(true);
}

// * This function reads data from a file descriptor until the end of HTTP headers is reached.
// * It appends the read data to the request_data string and returns the position just after the end of the headers.

std::string Request::readUntilHeadersEnd(int fd)
{
	char byte;
	int has_content;
	std::string request;

	while ((has_content = read(fd, &byte, 1)) > 0)
	{
		request += byte;
		if (request.find("\r\n\r\n") != std::string::npos) // If the end of the headers is found, break the loop
			break;
	}

	// Handle errors during reading
	if (has_content < 0) // If read returns a negative value, an error occurred
		throw errorReadingFD(500);
	else if (has_content == 0 && request.empty()) // If no data is read and request_data is empty, the client disconnected
		throw connectionCloseEarly(499);
	return (request);
}

void Request::prepareBodyParsing(int fd)
{
	if (_headers.find("Expect") != _headers.end())
	{
		const char *continueMsg = "HTTP/1.1 100 Continue\r\n\r\n";
		write(fd, continueMsg, strlen(continueMsg));
		sleep(1);
	}
	if (_headers.find("Content-Length") != _headers.end())
	{
		unsigned int length = std::atoi(_headers["Content-Length"].c_str());
		parseBody(fd, length);
	}
	else if (_headers.count("Transfer-Encoding") && _headers["Transfer-Encoding"] == "chunked")
		processChunkedBody(fd);
}

// * Function to parse an HTTP request from a given file descriptor.
// * It reads the request, parses the request line, headers, and body based on the headers.

void Request::parseRequest(int fd)
{
	std::string request_data = readUntilHeadersEnd(fd); // Read data until the end of the headers section
	std::istringstream header_stream(request_data);		// Create a stream for header parsing
	std::string line;
	std::getline(header_stream, line);

	parseRequestLine(line); // Parse the request line (e.g., GET /index.html HTTP/1.1)

	while (std::getline(header_stream, line) && !line.empty() && line != "\r" && line != "\r\n") // Loop to parse headers until an empty line or newline sequence
		parseHeaders(line);

	prepareBodyParsing(fd);
}

// ! WORKING VERSION DONT DELETE
// void Request::parseRequest(int fd)
// {
// 	char buffer[MESSAGE_BUFFER]; // Buffer to hold incoming data from the file descriptor
// 	std::string request_data; // String to accumulate the request data
// 	int has_content; // Variable to hold the number of bytes read from the file descriptor

// 	// Read from the file descriptor in a loop until there is no more data
// 	while ((has_content = read(fd, buffer, MESSAGE_BUFFER)) > 0)
// 	{
// 		// Append the read data to the request_data string
// 		request_data.append(buffer, has_content);
// 		// Clear the buffer
// 		memset(buffer, 0, has_content);

// 		// Find the end of the HTTP headers
// 		size_t header_end = request_data.find("\r\n\r\n");
// 		if (header_end != std::string::npos)
// 		{
// 			// Create a stream to process the headers
// 			std::istringstream header_stream(request_data.substr(0, header_end + 4));
// 			std::string line;
// 			// Parse the request line (e.g., GET /index.html HTTP/1.1)
// 			std::getline(header_stream, line);
// 			parseRequestLine(line);

// 			// Parse all the headers
// 			while (std::getline(header_stream, line) && !line.empty() && line != "\r" && line != "\r\n")
// 				parseHeaders(line);

// 			// Determine where the body starts
// 			size_t body_start = header_end + 4;

// 			// Handle the body if Content-Length is specified
// 			if (_headers.find("Content-Length") != _headers.end())
// 			{
// 				size_t content_length = std::stoi(_headers["Content-Length"]);
// 				if (request_data.size() >= body_start + content_length)
// 				{
// 					std::istringstream body_stream(request_data.substr(body_start, content_length));
// 					parseBody(body_stream);
// 					break;
// 				}
// 			}
// 			// Handle the body if Transfer-Encoding is chunked
// 			else if (_headers.count("Transfer-Encoding") && _headers["Transfer-Encoding"] == "chunked")
// 			{
// 				processChunkedBody(fd, request_data.substr(body_start));
// 				break;
// 			}
// 			// No body to process
// 			else
// 				break;
// 		}
// 	}

// 	// Handle errors during reading
// 	if (has_content < 0)
// 		throw std::runtime_error("Error reading from socket");
// 	else if (has_content == 0 && request_data.empty())
// 		throw std::runtime_error("Client disconnected");
// }

// * This function parses the URI to determine the current location and file path.
// * It extracts the location and file paths from the URI and sets the appropriate
// * member variables based on configuration and file system checks.

void Request::parseUri()
{
	int slash_pos;
	int end_pos;
	std::string temp_loc_path;
	std::string temp_file_path;
	std::string uri;
	uri = this->_uri;
	end_pos = sizeof(uri); // Calculate the size of the URI.
	slash_pos = end_pos;
	temp_loc_path = uri;

	while (1)
	{
		for (size_t i = 0; i < client->get_listen_socket().get_location().size(); ++i)
		{
			if (!client->get_listen_socket().get_location()[temp_loc_path].getName().empty())
			{
				this->_curr_loc = client->get_listen_socket().get_location()[temp_loc_path];
				break; // Exit the loop if a matching location is found.
			}
		}
		if (slash_pos <= 0 || !_curr_loc.getName().empty())
			break; // Stop if no more slashes or current location is set.
		else
		{
			slash_pos = temp_loc_path.find_last_of('/');					 // Find the last slash.
			temp_loc_path = uri.substr(0, slash_pos);						 // Update the location path.
			temp_file_path = uri.substr(slash_pos + 1, end_pos - slash_pos); // Extract the file path.
			if (slash_pos == 0)
			{
				temp_loc_path = "/";
				temp_file_path = uri.substr(slash_pos + 1, end_pos - slash_pos);
			}
		}
	}

	std::string temp_root = client->get_listen_socket().get_root();
	if (!_curr_loc.getRoot().empty())
		temp_root = _curr_loc.getRoot(); // Use current location root if available.

	_file_path = temp_file_path;
	if (!_curr_loc.getIndex().empty() && _file_path.empty())
	{
		_file_path = _curr_loc.getIndex(); // Set file path to index if empty.
		_full_path = temp_root + _curr_loc.getName() + "/" + _file_path;
		replaceDoubleSlashes(_full_path);
		if (access(_full_path.c_str(), F_OK) && _curr_loc.getAutoindex())
			_file_path.clear(); // Clear file path if file doesn't exist and autoindex is enabled.
	}

	if (_curr_loc.empty())
		_file_path = uri; // Use URI as file path if current location is empty.

	_full_path = temp_root + _curr_loc.getName() + "/" + _file_path;
	replaceDoubleSlashes(_full_path);
	if (!_file_path.empty() && isDirectory(_full_path))
		_file_path.clear(); // Clear file path if it's a directory.

	replaceDoubleSlashes(_full_path);
	if (!_file_path.empty() && isDirectory(_full_path))
		_file_path.clear(); // Clear file path again if it's still a directory.

	redirectInURI(); // Perform any necessary URI redirections.
}

// * Checks if a file at _full_path is accessible with the specified mode.
// * Throws a 404 error if the file is not found.

void Request::checkFile(int mode)
{
	if (access(_full_path.c_str(), mode))
		throw fileNotFound(404);
	return;
}

// * Checks if the HTTP method used in the request is allowed for the current location or root.
// * Throws a 405 error if the method is not allowed.

void Request::isMethodAllowed()
{
	std::vector<std::string> methods;

	// Get allowed methods based on the current location or root
	if (_curr_loc.empty())
		methods = client->get_listen_socket().get_allow_methods(); // Root allowed methods
	else
		methods = _curr_loc.getAllowMethods(); // Location-specific allowed methods

	// Check if the method is allowed
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); ++it)
	{
		if (_method == *it)
			return; // Method is allowed
	}

	throw unauthorizedMethod(405); // Throw exception if method is not allowed
}

// * Redirects the URI if the current location has a return directive.
// * Updates the URI and parses it again.

void Request::redirectInURI()
{
	if (_curr_loc.empty())
		return;

	// If the current location has a return URI, set it as the new URI
	if (!_curr_loc.getReturn().empty())
	{
		setURI(_curr_loc.getReturn());
		_curr_loc = location(); // Reset current location
		_hasReturn = true;
		_file_path.clear();
		_full_path.clear();
		parseUri(); // Parse the updated URI
	}
}

// * Checks if a given URI is a folder by examining the presence of '.' or '/'.
// * Returns true if it's a folder, false otherwise.

bool Request::checkfolder(std::string uri)
{
	for (unsigned int i = uri.size(); i != 0; i--)
	{
		if (uri[i - 1] == '.')
			return false; // URI contains a dot, not a folder
		if (uri[i - 1] == '/')
			return true; // URI contains a slash, indicating a folder
	}
	return false;
}

// * Replaces all instances of double slashes ("//") with a single slash ("/") in a given string.

void replaceDoubleSlashes(std::string &str)
{
	std::string::size_type pos = 0;
	// Find and replace double slashes
	while ((pos = str.find("//", pos)) != std::string::npos)
		str.replace(pos, 2, "/");
}

bool isDirectory(std::string &path)
{
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
	{
		std::cerr << "Cannot access " << path << std::endl;
		return false;
	}
	else if (info.st_mode & S_IFDIR)
	{
		return true; // C'est un répertoire
	}
	else
	{
		return false; // C'est un fichier
	}
}

// ------------------------------------------------- ERROR -- //

const char *Request::bodySize::what() const throw()
{
	return ("Body length exceeds server limits.");
}
const char *Request::wrongRLInput::what() const throw()
{
	return ("Request line arguments doesn't fit format.");
}
const char *Request::headerParsingError::what() const throw()
{
	return ("Header parsing error.");
}
const char *Request::invalidContentLength::what() const throw()
{
	return ("Negative length content specified.");
}
const char *Request::shorterBodyContent::what() const throw()
{
	return ("Mismatch between specified content-length and actuel content size (shorter).");
}
const char *Request::longerBodyContent::what() const throw()
{
	return ("Mismatch between specified content-length and actuel content size (longer).");
}
const char *Request::contentLengthUnspecified::what() const throw()
{
	return ("Content-length was not specified.");
}
const char *Request::unauthorizedMethod::what() const throw()
{
	return ("Unauthorized method requested.");
}
const char *Request::fileNotFound::what() const throw()
{
	return ("File not found");
}
const char *Request::unsupportedHTTPVersion::what() const throw()
{
	return ("Unsupported HTTP Version. Please conform to HTTP 1.1 only.");
}

const char *Request::chunkSizeError::what() const throw()
{
	return ("Failed to read chunk size.");
}
const char *Request::chunkDataError::what() const throw()
{
	return ("Failed to read chunk data.");
}
const char *Request::connectionCloseEarly::what() const throw()
{
	return ("Connexion closed by server.");
}
const char *Request::errorReadingFD::what() const throw()
{
	return ("Error while trying to read fd.");
}
