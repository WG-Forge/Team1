#include "client.h"

Client::Client() : service_(), socket_(service_)
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

Client::ResponseMessage Client::Move(size_t line_idx, int speed, size_t train_idx)
{
    ActionMessage actionMessage{};
    actionMessage.actionCode = Action::MOVE;

    rapidjson::Document data;
    data.SetObject();

    rapidjson::Document::AllocatorType &allocator = data.GetAllocator();
    data.AddMember("line_idx", line_idx, allocator);
    data.AddMember("speed", speed, allocator);
    data.AddMember("train_idx", train_idx, allocator);

    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    data.Accept(writer);

    actionMessage.data = strbuf.GetString();
    actionMessage.dataLength = actionMessage.data.length();
    Send(actionMessage);
    return ReceiveResponse();
}

Client::ResponseMessage Client::Upgrade(const std::vector<size_t> &posts, const std::vector<size_t> &trains)
{
    ActionMessage actionMessage{};
    actionMessage.actionCode = Action::UPGRADE;

    rapidjson::Document data;
    data.SetObject();

    rapidjson::Document::AllocatorType &allocator = data.GetAllocator();

    rapidjson::Value jposts(rapidjson::kArrayType);
    for (const auto &post : posts)
    {
        jposts.PushBack(post, allocator);
    }
    data.AddMember("posts", jposts, allocator);

    rapidjson::Value jtrains(rapidjson::kArrayType);
    for (const auto &train : trains)
    {
        jtrains.PushBack(train, allocator);
    }
    data.AddMember("trains", jtrains, allocator);

    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    data.Accept(writer);

    actionMessage.data = strbuf.GetString();
    actionMessage.dataLength = actionMessage.data.length();
    Send(actionMessage);
    return ReceiveResponse();
}

Client::ResponseMessage Client::Turn()
{
    ActionMessage actionMessage{};
    actionMessage.actionCode = Action::TURN;
    Send(actionMessage);
    return ReceiveResponse();
}

Client::ResponseMessage Client::Games()
{
    ActionMessage actionMessage{};
    actionMessage.actionCode = Action::GAMES;
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

ip::tcp::socket &Client::GetSocket()
{
    return socket_;
}
