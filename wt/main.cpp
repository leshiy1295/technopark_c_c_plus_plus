#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/WObject.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WCompositeWidget.h>
#include <Wt/Http/Response.h>
#include <Wt/AsioWrapper/asio.hpp>

#include "postprocessor.h"

#include <system_error>
#include <iostream>

class ServerApplication : public Wt::WApplication
{
public:
    ServerApplication(const Wt::WEnvironment& env)
            : Wt::WApplication(env)
    {
        root()->addWidget(std::make_unique<Wt::WText>("Your name, please ? "));
        nameEdit_ = root()->addWidget(std::make_unique<Wt::WLineEdit>()); // allow text input
        nameEdit_->setFocus();

        Wt::WPushButton *button = root()->addWidget(std::make_unique<Wt::WPushButton>("Greet me."));
        button->setMargin(5, Wt::Side::Left);
        button->clicked().connect(this, &ServerApplication::greet);

        root()->addWidget(std::make_unique<Wt::WBreak>());    // insert a line break
        greeting_ = root()->addWidget(std::make_unique<Wt::WText>()); // empty text

        useStyleSheet("httpserver.css");
    }

    void greet()
    {
        greeting_->setText("Hello there, " + nameEdit_->text());
    }

private:
    Wt::WText *greeting_;
    Wt::WLineEdit *nameEdit_;
};

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env)
{
    return std::make_unique<ServerApplication>(env);
}


int main(int argc, char **argv)
{
    Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);
    server.addEntryPoint(Wt::EntryPointType::Application,
                         std::bind(&createApplication, std::placeholders::_1));

    PostProcessor post("Wt post example");
    server.addResource(&post, "/posts");

    server.run();

    return 0;
}