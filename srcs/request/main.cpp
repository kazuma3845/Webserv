#include "request.hpp"

int main() {
    std::string requestFile =
        "POST /submit-form HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 61\r\n"
        // "Transfer-Encoding: chunked\r\n"
        "Connection: keep-alive\r\n"
        "\r\n"
        "name=John+Doe&age=23&city=New+York&email=john.doe@example.com";    
        
    Request req;
    req.parseRequest(requestFile);
    req.printRequest();
    return 0;
}