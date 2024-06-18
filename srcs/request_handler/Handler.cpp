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

void Handler::start()
{
	std::map<std::string, void (Handler::*)()>::iterator it = _methodFunctions.find(_request.getMethod());
	if (it != _methodFunctions.end())
		(this->*(it->second))();
	else
		throw unknownMethod(501);
}

void Handler::handlePost()
{
	std::cout << "Handling POST request" << std::endl;
	std::string postData = _request.getBody();
	std::ofstream outFile("post_data.txt", std::ios::app); // appends at the end of the file
	if (!outFile.is_open())
		throw postFailed(500);
	outFile << postData << std::endl;
	outFile.close();
	_response.setStatusCode(200);
	_response.setContentType("text/plain");
	_response.setBody("POST data received and processed.");
}

void Handler::handleGet()
{
	std::string filePath = _request.getFilePath();
	std::string extension = filePath.substr(filePath.find_last_of('.') + 1);

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
	_response.loadContent(_request.getFullPath());
	_response.setStatusCode(200);
}

void Handler::handleDelete()
{
	std::string filePath = _request.getFilePath();
	if (std::remove(filePath.c_str()) != 0)
		throw deletionFailed(500);
	_response.setStatusCode(200);
	_response.setContentType("text/plain");
	_response.setBody("Content deleted successfully");
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