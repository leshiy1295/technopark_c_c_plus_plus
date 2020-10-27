#include <iostream>
#include <string>

#include "client.h"
#include "otherclient.h"

struct Creator
{
  enum type  { HTTP, TCP };
  virtual Client* createClient(type &clientType)
  {
    if (clientType == HTTP)
      	return new HTTPClient();
    else if(clientType == TCP)
      	return new TCPClient();

    return nullptr;
  }
};


int main()
{
	std::string host = "127.0.0.1";
	Creator::type type = Creator::type::HTTP;
	std::string data = "aaa";

	Creator creator;
	Client *client = creator.createClient(type);

	client->connect(host);
	client->send(data);
	client->close();
}