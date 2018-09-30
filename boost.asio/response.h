#ifndef BOOST_ASIO_SERVER_RESPONSE_H
#define BOOST_ASIO_SERVER_RESPONSE_H

struct Response
{
    int status_code;
    std::string status_message;
    std::vector<std::string> headers;
    std::string body;
    std::string http_version;
};

inline std::ostream& operator<<(std::ostream& os, const Response& response)
{
    os << response.status_code << std::endl;
    os << response.http_version << std::endl;
    for (int i = 0; i < response.headers.size(); i++)
        os << response.headers[i] << std::endl;
    os << response.body << std::endl;

    return os;
}

inline std::string Response2String(const Response &response)
{
    std::stringstream ss;
    ss << response.http_version << " " <<  response.status_code << " " << response.status_message << "\r\n\r\n";
    ss << response.body << "\r\n\r\n";

    return ss.str();
}
#endif //BOOST_ASIO_SERVER_RESPONSE_H
