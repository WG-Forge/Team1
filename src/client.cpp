#include "client.h"

Client::Client(io_service &io_service) : socket_(io_service)
{
    ip::tcp::endpoint ep(ip::address::from_string("92.223.2.79"), 443);
    socket_.connect(ep);
}

Client::ResponseMessage Client::Login(const std::string &name, const std::string &password, const std::string &game,
                                      int num_turns, int num_players)
{
    ActionMessage actionMessage{};
    actionMessage.actionCode = Action::LOGIN;

    rapidjson::Document data;
    data.SetObject();

    rapidjson::Document::AllocatorType &allocator = data.GetAllocator();

    data.AddMember("name", rapidjson::Value(name.c_str(), name.size(), allocator), allocator);
    if (!password.empty())
    {
        data.AddMember("password", rapidjson::Value(password.c_str(), password.size(), allocator), allocator);
    }
    if (!password.empty())
    {
        data.AddMember("game", rapidjson::Value(game.c_str(), game.size(), allocator), allocator);
    }
    if (num_turns != -1)
    {
        data.AddMember("num_turns", num_turns, allocator);
    }
    if (num_players != 1)
    {
        data.AddMember("num_players", num_players, allocator);
    }
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    data.Accept(writer);

    actionMessage.data = strbuf.GetString();
    actionMessage.dataLength = actionMessage.data.length();
    Send(actionMessage);
    return ReceiveResponse();
}

Client::ResponseMessage Client::Player()
{
    ActionMessage actionMessage{};
    actionMessage.actionCode = Action::PLAYER;
    Send(actionMessage);
    return ReceiveResponse();
}

Client::ResponseMessage Client::Logout()
{
    ActionMessage actionMessage{};
    actionMessage.actionCode = Action::LOGOUT;
    Send(actionMessage);
    return ReceiveResponse();
}

Client::ResponseMessage Client::Map(int layer)
{
    ActionMessage actionMessage{};
    actionMessage.actionCode = Action::MAP;

    rapidjson::Document data;
    data.SetObject();

    rapidjson::Document::AllocatorType &allocator = data.GetAllocator();

    data.AddMember("layer", layer, allocator);

    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    data.Accept(writer);

    actionMessage.data = strbuf.GetString();
    actionMessage.dataLength = actionMessage.data.length();
    Send(actionMessage);
    return ReceiveResponse();
}

void Client::Send(const Client::ActionMessage &actionMessage)
{
    write(socket_, buffer(&actionMessage.actionCode, sizeof(actionMessage.actionCode)));
    write(socket_, buffer(&actionMessage.dataLength, sizeof(actionMessage.dataLength)));
    write(socket_, buffer(&actionMessage.data[0], actionMessage.dataLength));
}

Client::ResponseMessage Client::ReceiveResponse()
{
    ResponseMessage responseMessage;
    read(socket_, buffer(&responseMessage.result, sizeof(responseMessage.result)));
    read(socket_, buffer(&responseMessage.dataLength, sizeof(responseMessage.dataLength)));
    responseMessage.data.resize(responseMessage.dataLength);
    read(socket_, buffer(&responseMessage.data[0], responseMessage.dataLength));
    return responseMessage;
}