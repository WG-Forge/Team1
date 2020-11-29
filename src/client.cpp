#include "client.h"

Client::Client()
{
    ss_.connect(SocketAddress("wgforge-srv.wargaming.net", 443));
}

Client::ResponseMessage Client::Login(const std::string &name, const std::string &password, const std::string &game,
                                      int num_turns, int num_players)
{
    ActionMessage actionMessage{};
    actionMessage.actionCode = Action::LOGIN;
    Poco::JSON::Object::Ptr data = new Poco::JSON::Object;
    data->set("name", name);
    if (!password.empty())
    {
        data->set("password", password);
    }
    if (!password.empty())
    {
        data->set("game", game);
    }
    if (num_turns != -1)
    {
        data->set("num_turns", num_turns);
    }
    if (num_players != 1)
    {
        data->set("num_players", num_players);
    }
    std::ostringstream ss;
    Poco::JSON::Stringifier::stringify(data, ss);
    actionMessage.data = ss.str();
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
    Poco::JSON::Object::Ptr data = new Poco::JSON::Object;
    data->set("name", layer);
    std::ostringstream ss;
    Poco::JSON::Stringifier::stringify(data, ss);
    actionMessage.data = ss.str();
    actionMessage.dataLength = actionMessage.data.length();
    Send(actionMessage);
    return ReceiveResponse();
}

void Client::Send(const Client::ActionMessage &actionMessage)
{
    ss_.sendBytes(&actionMessage.actionCode, sizeof(actionMessage.actionCode));
    ss_.sendBytes(&actionMessage.dataLength, sizeof(actionMessage.dataLength));
    ss_.sendBytes(&actionMessage.data[0], actionMessage.dataLength);
}

Client::ResponseMessage Client::ReceiveResponse()
{
    ResponseMessage responseMessage;
    ss_.receiveBytes(&responseMessage.result, sizeof(responseMessage.result));
    ss_.receiveBytes(&responseMessage.dataLength, sizeof(responseMessage.dataLength));
    responseMessage.data.resize(responseMessage.dataLength);
    ss_.receiveBytes(&responseMessage.data[0], responseMessage.dataLength);
    return responseMessage;
}