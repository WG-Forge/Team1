#include "client.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

// const Config config;

int main()
{
    Client client;
    auto res = client.Login("Boris");
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