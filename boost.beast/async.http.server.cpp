
#include "async.http.server.h"

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <thread>

#include "connection.h"

/*
Response HandlerGetPost(const Request &request)
{
    // use data from request

    Response response;
    response.status_code = 200;
    response.status_message = "OK";
    response.http_version = "HTTP/1.1";
    response.body = "{\"userId\": 2, \"data\" : \"it's my first post\" }";
    return response;
}

Response HandlerGetUser(const Request &request)
{
    // use data from request

    Response response;
    response.status_code = 200;
    response.status_message = "OK";
    response.body = "{\"userId\": 2, \"nickname\" : \"Vinograduss\" }";
    return response;
}
*/

namespace server3 {

        server::server(const std::string& address, const std::string& port,
                       std::size_t thread_pool_size)
                : thread_pool_size_(thread_pool_size),
                  //signals_(io_context_),
                  acceptor_(net::make_strand(io_context_))
        {
            //request_router.addHandler("/post", HandlerGetPost);
            //request_router.addHandler("/user", HandlerGetUser);

            // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
            boost::asio::ip::tcp::resolver resolver(io_context_);
            boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(net::socket_base::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen();
        }

        void server::run()
        {
            start_accept();

            // Create a pool of threads to run all of the io_contexts.
            std::vector<boost::shared_ptr<std::thread> > threads;
            for (std::size_t i = 0; i < thread_pool_size_; ++i)
            {
                boost::shared_ptr<std::thread> thread(new std::thread(
                        boost::bind(&net::io_context::run, &io_context_)));
                threads.push_back(thread);
            }

            // Wait for all threads in the pool to exit.
            for (std::size_t i = 0; i < threads.size(); ++i)
                threads[i]->join();
        }

        void server::start_accept()
        {
            acceptor_.async_accept(
                    net::make_strand(io_context_),
                    beast::bind_front_handler(&server::handle_accept, shared_from_this()));
        }

        void server::handle_accept(beast::error_code e, tcp::socket socket)
        {
            if (!e)
            {
                std::cout << "accept!";

                auto c = std::make_shared<Connection>(std::move(socket));
                c->start();
            }

            start_accept();
        }

        void server::handle_stop()
        {
            io_context_.stop();
        }

} // namespace server3

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[])
{
    try
    {
        // Check command line arguments.
        if (argc != 4)
        {
            std::cerr << "Usage: http_server <address> <port> <threads>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80 1 \n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80 1 \n";
            return 1;
        }

        // Initialise the server.
        std::size_t num_threads = boost::lexical_cast<std::size_t>(argv[3]);
        auto s = std::make_shared<server3::server>(argv[1], argv[2], num_threads);

        // Run the server until stopped.
        s->run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}