#ifndef BOOST_ASIO_SERVER_REQUEST_H
#define BOOST_ASIO_SERVER_REQUEST_H

struct Request
{
    std::string host;
    std::string method;
    std::string path;
    std::vector<std::string> params;
    std::vector<std::string> headers;
    std::string body;
    std::string http_version;
};

inline std::ostream& operator<<(std::ostream& os, const Request& request)
{
    os << request.method << std::endl;
    os << request.http_version << std::endl;
    for (int i = 0; i < request.headers.size(); i++)
        os << request.headers[i] << std::endl;
    os << request.body << std::endl;

    return os;
}

inline std::string Request2String(const Request &request)
{
    std::stringstream ss;
    ss << request.method << " " <<  request.path << " " << request.http_version << "\r\n";
    ss << "Host: " << request.host << "\r\n";

    for (int i = 0; i < request.headers.size(); i++)
    {
        ss << request.headers[i] << "\r\n";
    }

    ss << "\r\n";

    if (!request.body.empty())
        ss << request.body << "\r\n\r\n";

    return ss.str();
}

#endif //BOOST_ASIO_SERVER_REQUEST_H
