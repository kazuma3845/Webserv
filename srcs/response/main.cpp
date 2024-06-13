// #include "response.hpp"
// #include <iostream>
// #include <sstream>

// int main()
// {
// 	// Create a Response object
// 	Response response;

// 	// Set values
// 	response.setHTTPVersion("HTTP/1.1");
// 	response.setStatusCode(200);
// 	response.setStatusMessage("OK");

// 	std::istringstream headersStream("Content-Type: text/html\nContent-Length: 123");
// 	try
// 	{
// 		response.setHeaders(headersStream);
// 	}
// 	catch (std::exception &e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
// 	response.setBody("Hello, world!");
// 	response.printResponse();
// 	return 0;
// }