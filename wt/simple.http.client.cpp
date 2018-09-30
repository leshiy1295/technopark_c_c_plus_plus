#include <Wt/Http/Client.h>
#include <Wt/Http/Message.h>
#include <Wt/WObject.h>

#include "Wt/AsioWrapper/asio.hpp"

#include <system_error>
#include <iostream>

void handleHttpResponse(Wt::AsioWrapper::error_code err, const Wt::Http::Message &response)
{
    std::cout << "status:" << response.status();
    if (!err && response.status() == 200) {
        std::cout  << "Status code " << response.status() << "\n\n"
           << response.body();
    }
}

int main()
{
    Wt::AsioWrapper::asio::io_service ios;

    auto client = std::make_unique<Wt::Http::Client>(ios);
    client->setTimeout(std::chrono::seconds{15});
    client->setMaximumResponseSize(10 * 1024);
    client->done().connect(std::bind(handleHttpResponse, std::placeholders::_1, std::placeholders::_2));
    client->get("http://jsonplaceholder.typicode.com/posts/1");

    ios.run();

    return 0;
}