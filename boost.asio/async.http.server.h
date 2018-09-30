#ifndef BOOST_ASIO_SERVER_ASYNC_HTTP_SERVER_H
#define BOOST_ASIO_SERVER_ASYNC_HTTP_SERVER_H

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "connection.h"
#include "request.h"
#include "response.h"
#include "router.h"

namespace http {
    namespace server3 {

        class server
                : private boost::noncopyable
        {
        public:
            explicit server(const std::string& address, const std::string& port,
                            std::size_t thread_pool_size);

            /// Run the server's io_context loop.
            void run();

        private:
            /// Initiate an asynchronous accept operation.
            void start_accept();

            /// Handle completion of an asynchronous accept operation.
            void handle_accept(const boost::system::error_code& e);

            /// Handle a request to stop the server.
            void handle_stop();

            /// The number of threads that will call io_context::run().
            std::size_t thread_pool_size_;

        private:

            /// The io_context used to perform asynchronous operations.
            boost::asio::io_context io_context_;

            /// The signal_set is used to register for process termination notifications.
            boost::asio::signal_set signals_;

            /// Acceptor used to listen for incoming connections.
            boost::asio::ip::tcp::acceptor acceptor_;

            /// The next connection to be accepted.
            boost::shared_ptr<Connection> new_connection_;

            /// The handler for all incoming requests.
            Router<Response(*)(const Request &request)> request_router;
        };

    } // namespace server3
} // namespace http

#endif //BOOST_ASIO_SERVER_ASYNC_HTTP_SERVER_H
