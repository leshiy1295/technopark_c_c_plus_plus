#include <iostream>

#include "client.h"
#include "httpclient.hpp"



int main()
{

  HTTPNetworkWorker worker;
  worker.request("127.0.0.1", "I love you");

  return 0;
}