#ifndef SERVER_POSTPROCESSOR_H
#define SERVER_POSTPROCESSOR_H

#include <Wt/WResource.h>



class PostProcessor : public Wt::WResource
{
public:
    PostProcessor(const std::string& title)
            :    title_(title)
    {}

    virtual ~PostProcessor()
    {

    }

protected:
    virtual void handleRequest(const Wt::Http::Request &request,
                               Wt::Http::Response &response)
    {
        response.setStatus(200);
        response.setMimeType("application/json");
        response.out() << "[{\"userId\": 1, \"nickname\" : \"Vinograduss\" }, {\"userId\": 2, \"nickname\" : \"lambda\" } ]";
    }

private:
    std::string title_;
};

#endif //SERVER_POSTPROCESSOR_H
