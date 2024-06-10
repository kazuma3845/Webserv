#include "request.hpp"

Request::Request()
{
	std::cout << "Request instance created." << std::endl;
}

Request::~Request()
{
	std::cout << "Request instance destroyed ..." << std::endl;
}

void Request::printRequest()
{
	std::cout << "Method : " << method << std::endl;
	std::cout << "URI : " << uri << std::endl;
	std::cout << "Version :" << httpVersion << std::endl;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		std::cout << it->first << ":" << it->second << std::endl;
	if (!body.empty())
		std::cout << body << std::endl;
}

void Request::parseRequestLine(std::string line)
{
	std::istringstream ss(line);

	ss >> method >> uri >> httpVersion;

	if (method.empty() || uri.empty() || httpVersion.empty())
		throw wrongRLInput();
	// if (AllowdMethods != method) //A modifier pour checker les methodes autorisées par la config du serveur
	// 	throw unauthorizedMethod();
}
void Request::parseHeaders(std::string line)
{
	std::istringstream ss(line);
	std::string key, value;

	if (std::getline(ss, key, ':') && std::getline(ss, value))
		headers[key] = value;
	else
		throw headerParsingError();
}

void Request::parseBody(std::istringstream &ss)
{
	std::stringstream to_convert(headers["content-length"]); // Il se peut qu'il n'y ait pas de content length ...
	int length;
	to_convert >> length;												// conversion en int en utilisant le stream
	std::streamsize buffer_size = static_cast<std::streamsize>(length); // cast pour pouvoir le passer a read
	char buffer[buffer_size + 1];

	// lecture du nombre d'octets definis en amont
	ss.read(buffer, buffer_size);
	std::size_t bytes_read = ss.gcount(); // nombre de bytes reelement lus
	buffer[bytes_read] = '\0';

	body.append(&buffer[0], bytes_read); // conversion et placement dans le body
}

void Request::parseRequest(std::string requestFile)
{
	std::istringstream ss(requestFile);
	std::string line;
	try
	{
		// extraction de la première ligne qui est la RequestLine
		std::getline(ss, line);
		parseRequestLine(line);

		// lecture des headers jusqu'à /r
		 while (std::getline(ss, line) && line != "\r" && line != "\r\n" && line != "\n" && !line.empty())  //"\r" fais séparation entre les headers et le body
			parseHeaders(line);
		std::getline(ss, line);
		// lecture du body si il reste qqchose dans le stringstream
		if (!line.empty())
			{std::cout << "Current line is : " << line << std::endl;
			std::cout << "Shoudl start parsing body." << std::endl;}
		// parseBody(ss);
	}
	catch (std::exception &e)
	{
		std::cout << "Request parsing error : " << e.what() << std::endl;
	}
}

const char *Request::bodySize::what() const throw()
{
	return ("Body length exceeds server limits.");
}

const char *Request::wrongRLInput::what() const throw()
{
	return ("Request line arguments doesn't fit format.");
}

const char *Request::unauthorizedMethod::what() const throw()
{
	return ("Unauthorized method requested.");
}

const char *Request::headerParsingError::what() const throw()
{
	return ("Header parsing error.");
}

void Request::parseUri(void)
{
	int slash_pos;
	int end_pos;
	std::string temp_loc_path;
	std::string temp_file_path;

//      /doc/doc2/pipou.txt

	_full_path = this->uri;
	end_pos = sizeof(_full_path);
	slash_pos = _full_path.find_last_of('/');
	if (slash_pos < end_pos)
	{
		temp_loc_path = _full_path.substr(0, slash_pos - 1);
		temp_file_path = _full_path.substr(slash_pos + 1, end_pos);
	}
	while (1)
	{
		for (std::vector<location>::iterator it = _client->get_listen_socket()->get_location().begin(); it != _client->get_listen_socket()->get_location().end(); ++it)
		{
			if (it->getName() == temp_file_path)
				this->_curr_loc = &(*it);
		}
		if (slash_pos <= 0)
			break ;
		if (this->_curr_loc == nullptr)
		{
			slash_pos = temp_loc_path.find_last_of('/');
			temp_loc_path = _full_path.substr(0, slash_pos);
			temp_file_path = _full_path.substr(slash_pos, end_pos);
		}
		else
			break ;
	}
	_full_path = _client->get_listen_socket()->get_root() + _full_path;
}