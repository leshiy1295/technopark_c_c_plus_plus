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

class NetworkWorker
{
public:
	virtual Client *createClient() = 0;

	bool request(const std::string &host, const std::string &data)
	{
		Client *client = createClient();

		client->connect(host);
		client->send(data);
		client->close();

		delete client;

		return true;
	}
};

#endif