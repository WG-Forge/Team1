#include "client.h"
#include <iostream>

// const Config config;

int main()
{
    io_service service;
    Client client(service);
    client.Login("Leha");
    auto res = client.Map(1);
    rapidjson::Document document;
    document.Parse(res.data.c_str());
    rapidjson::StringBuffer buffer{};
    rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
    document.Accept(writer);
    std::cout << buffer.GetString() << '\n';
    return 0;
    //    Application app(config);
    //    return app.run();
}