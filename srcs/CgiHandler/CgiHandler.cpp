#include "CgiHandler.hpp"

CgiHandler::CgiHandler(Request &request) : _body(request.getBody())
{
}

CgiHandler::~CgiHandler()
{
}

std::string CgiHandler::execute()
{
	return "";
}
