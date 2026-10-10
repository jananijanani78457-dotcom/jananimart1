
#include <drogon/drogon.h>
#include <iostream>

int main()
{
    std::cout << "JananiMart HTTP server starting..."
              << std::endl;

    drogon::app().addListener("127.0.0.1", 8080);

    drogon::app().registerHandler(
        "/",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback)
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setBody("JananiMart backend is running!");
            callback(response);
        },
        {drogon::Get}
    );

    drogon::app().run();
    return 0;
}
