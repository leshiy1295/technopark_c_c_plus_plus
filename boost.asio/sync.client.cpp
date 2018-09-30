
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <map>
#include <boost/asio.hpp>

#include "response.h"
#include "request.h"

using boost::asio::ip::tcp;

class Client
{
public:
    Client(boost::asio::io_context &io_context)
    : io_context_(io_context)
    {
    }
    Response sendRequest(const std::string& server, const std::string& path)
    {
        tcp::resolver resolver(io_context_);
        tcp::resolver::results_type endpoints = resolver.resolve(server, "http");

        tcp::socket socket(io_context_);
        boost::asio::connect(socket, endpoints);

        Request request;
        request.method = "GET";
        request.path = path;
        request.http_version = "HTTP/1.0";
        request.host = server;
        std::string buffer = Request2String(request);

        // Send the request.
        boost::asio::write(socket, boost::asio::buffer(buffer.data(), buffer.size()));

        // Read the response status line. The response streambuf will automatically
        boost::asio::streambuf response_buf;
        boost::asio::read_until(socket, response_buf, "\r\n");

        // Check that response is OK.
        Response response;
        std::istream response_stream(&response_buf);
        response_stream >> response.http_version;
        response_stream >> response.status_code;
        std::getline(response_stream, response.status_message);

        if (response.status_code != 200)
        {
            std::cout << "Response returned with status code " << response.status_code << "\n";
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response_buf, "\r\n\r\n");

        // Process the response headers.
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
            response.headers.push_back(header);

        // Read until EOF, writing data to output as we go.
        boost::system::error_code error;
        boost::asio::read(socket, response_buf, boost::asio::transfer_all(), error);
        response.body = std::string(std::istreambuf_iterator<char>(response_stream), std::istreambuf_iterator<char>());

        return response;
    }

private:
    boost::asio::io_context &io_context_;
};
int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cout << "Usage: sync_client <server> <path>\n";
            std::cout << "Example:\n";
            std::cout << "  sync_client www.boost.org /LICENSE_1_0.txt\n";
            return 1;
        }

        boost::asio::io_context io_context;

        Client api_client = Client(io_context);
        Response res = api_client.sendRequest(argv[1], argv[2]);

        std::cout << "Response \n" << res;
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}
