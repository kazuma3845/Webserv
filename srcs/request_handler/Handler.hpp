#pragma once
#include "../response/response.hpp"
#include "../request/request.hpp"
#include "../errors/ErrorWebServ.hpp"

class Handler
{
public:
	Handler(Request &request, Response &response);
	~Handler();

	void start();

private:
	// ----------------------------------------- ATTRIBUTES //
	Request &_request;
	Response &_response;
	std::map<std::string, void (Handler::*)()> _methodFunctions;

	// ----------------------------------------- FUNCTIONS //

	void handleGet();
	void handlePost();
	void handleDelete();
	std::string extractBoundary(const std::string& contentType);
	bool processPart(std::istringstream& stream);

	// ------------------------------------------ ERRORS //

	class unknownMethod : public ErrorWebServ
	{
	public:
		unknownMethod(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};

	class deletionFailed : public ErrorWebServ
	{
	public:
		deletionFailed(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};

	class postFailed : public ErrorWebServ
	{
	public:
		postFailed(int errorCode) : ErrorWebServ(errorCode) {}
		const char *what() const throw();
	};
};