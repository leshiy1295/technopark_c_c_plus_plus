#include <string>

class TCPClient : public Client
{
public:
	bool connect(const std::string &host) { std::cout << "coonect with: " << host << std::endl; return true;}
	bool send(const std::string &data) { std::cout << "send data: " << data << std::endl; return true;}
	bool close() { std::cout << "close connection  " << std::endl; return true;}
	~TCPClient() {}
};

class HTTPClient : public Client
{
public:
	bool connect(const std::string &host) { std::cout << "coonect with: " << host << std::endl; return true;}
	bool send(const std::string &data) { std::cout << "send data: " << data << std::endl; return true;}
	bool close() { std::cout << "close connection  " << std::endl; return true;}
	~HTTPClient() {}
};