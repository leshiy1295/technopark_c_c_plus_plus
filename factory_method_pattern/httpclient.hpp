#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H 
 
#include <string>
#include "client.h"

// ConcreteProduct
class HTTPClient : public Client
{
public:
  bool connect(const std::string &host) { std::cout << "coonect with: " << host << std::endl; return true;}
  bool send(const std::string &data) { std::cout << "send data: " << data << std::endl; return true;}
  bool close() { std::cout << "close connection  " << std::endl; return true;}
  
  ~HTTPClient() {}
};

// ConcreteCreator
class HTTPNetworkWorker : public NetworkWorker
{
public:
    virtual Client* createClient()
    {
       return new HTTPClient();
    }
};

#endif