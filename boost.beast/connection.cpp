#include "connection.h"

#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/beast/http/write.hpp>
#include <iostream>

#include <string>
#include <sstream>

namespace server3 {

        Connection::Connection(tcp::socket&& socket)
                : stream_(std::move(socket))
        {
        }


        void Connection::start()
        {
            net::dispatch(stream_.get_executor(),
                          beast::bind_front_handler(&Connection::do_read,
                                             shared_from_this()));

        }

        void Connection::do_read()
        {
            request_ = {};

            // Read a request
            http::async_read(stream_, buffer_, request_,
                             beast::bind_front_handler(
                                     &Connection::handle_read,
                                     shared_from_this()));
        }

        void Connection::handle_read(beast::error_code e,
                                     std::size_t bytes_transferred)
        {
            if( e == http::error::end_of_stream)
                return do_close();

            if (!e)
            {
                http::response<http::string_body> res{http::status::bad_request, request_.version()};
                if (request_.target() == "/posts")
                {
                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.set(http::field::content_type, "application/json");
                    res.result(http::status::ok);
                    res.body() = "{\"userId\": 2, \"data\" : \"it's my first post\" }";

                }
                else
                {
                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.set(http::field::content_type, "application/json");
                    res.result(http::status::not_found);
                    res.body() = " Not found";
                }

                //res.prepare_payload();

                auto sp = std::make_shared<http::message<false, http::string_body>>(std::move(res));
                res_ = sp;

                http::async_write(stream_,
                        *sp,
                        beast::bind_front_handler(
                                &Connection::handle_write,
                                shared_from_this(),
                                sp->need_eof()));
            }
        }

        void Connection::handle_write(bool close,
                                      beast::error_code e,
                                      std::size_t bytes_transferred)
        {
            if(close)
            {
                // This means we should close the connection, usually because
                // the response indicated the "Connection: close" semantic.
                return do_close();
            }

            if (!e)
            {
                do_read();
            }
        }

        void Connection::do_close()
        {
            beast::error_code ec;
            stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

        }

    } // namespace server3

