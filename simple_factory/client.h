#ifndef CLIENT_H
#define CLIENT_H 
 
#include <string>

class Client
{
public:
	virtual bool connect(const std::string &host) = 0;
	virtual bool send(const std::string &data) = 0;
 	virtual bool close() = 0;
	virtual ~Client() {};
};

#endif