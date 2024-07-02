#include "response.hpp"

//////////////////////// -------- COPLIEN DECLARATION -------- ////////////////////////////

Response::Response()
{
	_statusCode = 200;
	_statusMessage = "OK";
	// Default constructor implementation
}

Response::Response(const Response &other)
{
	// Copy constructor implementation
	*this = other;
}

Response &Response::operator=(const Response &other)
{
	if (this != &other)
	{
		_httpVersion = other._httpVersion;
		_statusCode = other._statusCode;
		_statusMessage = other._statusMessage;
		_headers = other._headers;
		_body = other._body;
		_resp = other._resp;
		_contentType = other._contentType;
		_connectionType = other._connectionType;
	}
	return *this;
}


Response::~Response()
{
	// Destructor implementation
}

////////////////////////////////////// -------- GETTERS -------- ////////////////////////////////

std::string Response::getHTTPVersion()
{
	return (_httpVersion);
}

int Response::getStatusCode()
{
	return (_statusCode);
}

std::string Response::getStatusMessage()
{
	return (_statusMessage);
}

// std::map<std::string, std::string> Response::getHeaders()
// {
// 	return (_headers);
// }

std::string Response::getBody()
{
	return (_body);
}

std::string Response::getResp()
{
	return (_resp);
}

////////////////////  -------------------------- SETTERS -------------------------- //

void Response::setHTTPVersion(std::string version)
{
	this->_httpVersion = version;
}
void Response::setStatusCode(int code)
{
	this->_statusCode = code;

	switch (code)
	{
	case 100:
		this->_statusMessage = "Continue";
		break;
	case 101:
		this->_statusMessage = "Switching Protocols";
		break;
	case 200:
		this->_statusMessage = "OK";
		break;
	case 201:
		this->_statusMessage = "Created";
		break;
	case 202:
		this->_statusMessage = "Accepted";
		break;
	case 203:
		this->_statusMessage = "Non-Authoritative Information";
		break;
	case 204:
		this->_statusMessage = "No Content";
		break;
	case 205:
		this->_statusMessage = "Reset Content";
		break;
	case 206:
		this->_statusMessage = "Partial Content";
		break;
	case 300:
		this->_statusMessage = "Multiple Choices";
		break;
	case 301:
		this->_statusMessage = "Moved Permanently";
		break;
	case 302:
		this->_statusMessage = "Found";
		break;
	case 303:
		this->_statusMessage = "See Other";
		break;
	case 304:
		this->_statusMessage = "Not Modified";
		break;
	case 305:
		this->_statusMessage = "Use Proxy";
		break;
	case 307:
		this->_statusMessage = "Temporary Redirect";
		break;
	case 308:
		this->_statusMessage = "Permanent Redirect";
		break;
	case 400:
		this->_statusMessage = "Bad Request";
		break;
	case 401:
		this->_statusMessage = "Unauthorized";
		break;
	case 403:
		this->_statusMessage = "Forbidden";
		break;
	case 404:
		this->_statusMessage = "Not Found";
		break;
	case 405:
		this->_statusMessage = "Method Not Allowed";
		break;
	case 406:
		this->_statusMessage = "Not Acceptable";
		break;
	case 407:
		this->_statusMessage = "Proxy Authentication Required";
		break;
	case 408:
		this->_statusMessage = "Request Timeout";
		break;
	case 409:
		this->_statusMessage = "Conflict";
		break;
	case 410:
		this->_statusMessage = "Gone";
		break;
	case 411:
		this->_statusMessage = "Length Required";
		break;
	case 412:
		this->_statusMessage = "Precondition Failed";
		break;
	case 413:
		this->_statusMessage = "Payload Too Large";
		break;
	case 414:
		this->_statusMessage = "URI Too Long";
		break;
	case 415:
		this->_statusMessage = "Unsupported Media Type";
		break;
	case 416:
		this->_statusMessage = "Range Not Satisfiable";
		break;
	case 417:
		this->_statusMessage = "Expectation Failed";
		break;
	case 426:
		this->_statusMessage = "Upgrade Required";
		break;
	case 500:
		this->_statusMessage = "Internal Server Error";
		break;
	case 501:
		this->_statusMessage = "Not Implemented";
		break;
	case 502:
		this->_statusMessage = "Bad Gateway";
		break;
	case 503:
		this->_statusMessage = "Service Unavailable";
		break;
	case 504:
		this->_statusMessage = "Gateway Timeout";
		break;
	case 505:
		this->_statusMessage = "HTTP Version Not Supported";
		break;
	default:
		this->_statusMessage = "Unknown Status";
		break;
	}
}

void Response::setStatusMessage(std::string message)
{
	this->_statusMessage = message;
}

// void Response::setHeaders(std::istringstream &ss)
// {
// 	std::string key;
// 	std::string value;
// 	while (std::getline(ss, key, ':') && std::getline(ss, value))
// 		_headers[key] = value;
// 	if (ss.get() != EOF)
// 		throw settingHeadersError(400); // FIXME: need to be corrected
// }

void Response::setBody(std::string body)
{
	this->_body = body;
}

void Response::setContentType(std::string type)
{
	_contentType = type;
}

void Response::setConnectionType(bool status)
{
	if (status == true)
		_connectionType = "keep-alive";
	else
		_connectionType = "close";
}

void Response::printResponse() const
{
	std::cout << "HTTP Version: " << _httpVersion << std::endl;
	std::cout << "Status Code: " << _statusCode << " " << _statusMessage << std::endl;
	std::cout << "Headers:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Body:" << _body << std::endl;
}

////////////////////// --------------- ADDITIONNAL FUNCTIONS ---------------- //////////////////////////

void Response::loadContent(const std::string &filePath)
{
	std::ifstream file(filePath.c_str());
	if (!file)
		throw cantLoadContent(404);
	else
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		_body = buffer.str();
	}
}

std::string Response::takeTime() const
{
	time_t raw_time;
	struct tm *time_info;
	char buffer[80];

	time(&raw_time);
	time_info = localtime(&raw_time);

	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %Z", time_info);
	std::string str_buffer(buffer);
	return str_buffer;
}

void Response::formatResponse(Client &a, Request &b)
{
	std::string loc_path;
	if (!b.getFullPath().empty())
	{
		loc_path = b.getFullPath().substr(a.get_listen_socket().get_root().size(), b.getFullPath().size());
	}
	std::stringstream ss;
	ss << _statusCode;
	_resp += "HTTP/1.1 " + ss.str() + " " + _statusMessage + "\r\n"
															 "Date: " +
			 takeTime() + "\r\n"
						  "Content-Type: " +
			 _contentType + "\r\n";
	ss.str("");
	ss << a.get_listen_socket().get_port();
	_resp += "Location: http://" + a.get_listen_socket().get_host() + ":" + ss.str() + "/" + loc_path + "\r\n"
																										"Connection: " +
			 _connectionType + "\r\n";
	ss.str("");
	ss << _body.size();
	_resp += "Content-Length: " + ss.str() + "\r\n"
											 "\r\n";
	_resp += _body;
}

void Response::ErrorBody(int error_code, Client &a, bool b)
{
	std::string path;
	std::stringstream ss;
	if (b == false)
	{
		ss << error_code;
		if (error_code % 400 < 100)
			path = "Page/error/400/" + ss.str() + ".html";
		else if (error_code % 500 < 100)
			path = "Page/error/500/" + ss.str() + ".html";
		ss.str("");
	}
	else
		path = a.get_listen_socket().get_error_path()[a.get_listen_socket().get_error()];
	std::ifstream file(path.c_str());
	if (!file)
	{
		std::cerr << "Failed to open file: " << path << '\n';
		ErrorBody(error_code, a, false);
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	_body = buffer.str();
}

////////////////////// --------------- ERRORS ---------------- //////////////////////////

const char *Response::settingHeadersError::what() const throw()
{
	return ("Setting headers durign response failed.");
}

const char *Response::cantLoadContent::what() const throw()
{
	return ("Could not load content from path.");
}
