#include "Handler.hpp"

Handler::Handler(Request &request, Response &response) : _request(request), _response(response)
{
	_methodFunctions["GET"] = &Handler::handleGet;
	_methodFunctions["POST"] = &Handler::handlePost;
	_methodFunctions["DELETE"] = &Handler::handleDelete;
}

Handler::~Handler()
{
	// Destructor implementation
}

// ------------------------------------------------------ FUNCTIONS //

// * Starts the request handling process by invoking the appropriate method based on the HTTP request method.
// * Looks up the method in a map and calls the corresponding member function if found.

void Handler::start()
{
	std::cout << "Current METHOD is " << _request.getMethod() << std::endl;
	// Find the request method in the map of method functions
	std::map<std::string, void (Handler::*)()>::iterator it = _methodFunctions.find(_request.getMethod());
	if (it != _methodFunctions.end()) // If method is found, call the corresponding function
		(this->*(it->second))();
	else
		throw unknownMethod(501);
}

// * Handles HTTP POST requests by processing form data or file uploads.
// * For "application/x-www-form-urlencoded" content type, it saves the form data to a file.
// * For "multipart/form-data" content type, it processes each file part and saves it.

void Handler::handlePost()
{
	std::string contentType = _request.getHeaders()["Content-Type"];

	if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
	{
		std::string postData = _request.getBody();
		std::string outputPath = "Page/data/post_data.txt"; // Predefined file path

		// Open file to append post data
		std::ofstream outFile(outputPath.c_str(), std::ios::app);
		if (!outFile.is_open())
			throw postFailed(500);
		outFile << postData << std::endl;
		outFile.close();
		_response.setStatusCode(201);
		_response.setContentType("text/plain");
		_response.setBody("POST data received and written.");
		_request.getClient()->setFlag(WRITING_RESPONSE);
	}
	else if (contentType.find("multipart/form-data") != std::string::npos)
	{
		std::string boundary = extractBoundary(contentType);
		std::istringstream stream(_request.getBody());
		std::string partContent;
		bool hasSuccessfulPart = false;
		while (getNextPart(stream, boundary, partContent)) // Read and process each part of the multipart data
		{
			if (processPart(partContent, boundary))
				hasSuccessfulPart = true;
			else
				throw postFailed(500);
		}
		if (hasSuccessfulPart)
		{
			_response.setStatusCode(201);
			_response.setContentType("text/plain");
			_response.setBody("Files uploaded successfully.");
			_request.getClient()->setFlag(WRITING_RESPONSE);
		}
		else
			throw postFailed(500); // No part processed successfully
	}
	else
		throw unsupportedMediaType(415); // Unsupported Media Type
}

// * Extracts the next part of content delimited by a specified boundary from a stringstream.

bool Handler::getNextPart(std::istringstream &stream, const std::string &boundary, std::string &partContent)
{
	partContent.clear(); // Clear the output parameter to prepare for new content.
	std::string line;
	bool inPart = false; // Indicates if currently inside the desired content part.

	while (std::getline(stream, line))
	{ // Read lines from the stringstream.
		if (line.find(boundary) != std::string::npos)
		{ // Check if the boundary is found in the current line.
			if (inPart)
				return true;
			inPart = true; // Mark that we are now inside a new part.
		}
		else if (inPart)
		{
			partContent += line + "\n"; // Append the line to partContent if inside the desired part.
		}
	}
	return !partContent.empty(); // Return true if partContent is not empty (indicating a part was found).
}

std::string Handler::extractBoundary(const std::string &contentType)
{
	std::size_t pos = contentType.find("boundary=");
	if (pos != std::string::npos)
		return "--" + contentType.substr(pos + 9); // 9 pour passer 'boundary=' et ajouter '--' pour correspondre au format des délimiteurs
	return "";
}

// * This function processes a part of a multipart message.
// * It extracts the filename from the headers, and writes the content to a file in the specified directory.

bool Handler::processPart(const std::string &partContent, const std::string &boundary)
{
	std::istringstream stream(partContent);
	std::string line;
	std::string filename;
	bool inHeader = true; // Indicates whether the current line is part of the headers

	// Parse headers to get the filename
	while (inHeader && std::getline(stream, line))
	{
		if (line.find("Content-Disposition:") != std::string::npos)
		{
			std::size_t pos = line.find("filename=");
			if (pos != std::string::npos)
			{
				filename = line.substr(pos + 10);					  // Extract filename starting after 'filename="'
				filename = filename.substr(0, filename.length() - 2); // Remove the last character (usually a quote)
			}
		}
		if (line == "\r" || line == "\n" || line.empty()) // End of headers
			inHeader = false;
	}

	if (filename.empty())
		return false; // Return false if filename not found

	// Open the file to write
	std::string outputPath = "Page/data/" + filename;
	std::ofstream outFile(outputPath.c_str(), std::ios::binary);
	if (!outFile.is_open())
		return false; // Return false if file cannot be opened

	// Read the content of the file and write to outFile
	while (std::getline(stream, line))
	{
		// Check if the line contains the end of the part (delimited by "--")
		if (line.find(boundary) != std::string::npos)
			break;
		outFile << line << "\n"; // Write the line to the file
	}
	outFile.close();
	return true;
}

// * This function handles a GET request by determining the file extension of the requested file,
// * setting the appropriate content type in the response, loading the file content into the response,

void Handler::handleGet()
{
	std::string filePath = _request.getFilePath();							 // Get the file path from the request object.
	std::string extension = filePath.substr(filePath.find_last_of('.') + 1); // Extract the file extension.

	// Determine and set the content type based on the file extension.
	if (extension == "html")
		_response.setContentType("text/html");
	else if (extension == "css")
		_response.setContentType("text/css");
	else if (extension == "js")
		_response.setContentType("application/javascript");
	else if (extension == "json")
		_response.setContentType("application/json");
	else if (extension == "jpg" || extension == "jpeg")
		_response.setContentType("image/jpeg");
	else if (extension == "png")
		_response.setContentType("image/png");
	else
		_response.setContentType("text/plain");
	_response.loadContent(_request.getFullPath()); // Load the content of the requested file into the response.
	_response.setStatusCode(200);
	_request.getClient()->setFlag(WRITING_RESPONSE);
}

// * This function handles a DELETE request

void Handler::handleDelete()
{
	std::string filePath = _request.getFullPath();
	if (std::remove(filePath.c_str()) != 0)
		throw deletionFailed(500);
	_response.setStatusCode(204);
	_response.setContentType("text/plain");
	_response.setBody("Content deleted successfully.");
	_request.getClient()->setFlag(WRITING_RESPONSE);
}
Request Handler::getRequest()
{
	return (_request);
}
void Handler::setRequest(Request &req)
{
	_request = req;
}

// ------------------------------------------------------ ERRORS //

const char *Handler::unknownMethod::what() const throw()
{
	return ("Unhandled method.");
}
const char *Handler::deletionFailed::what() const throw()
{
	return ("DELETE failed.");
}

const char *Handler::postFailed::what() const throw()
{
	return ("POST processing failed.");
}

const char *Handler::unsupportedMediaType::what() const throw()
{
	return ("Unsupported media type during POST.");
}