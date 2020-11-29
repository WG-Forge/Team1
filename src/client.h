#ifndef RAIL_SIMULATOR_CLIENT_H
#define RAIL_SIMULATOR_CLIENT_H

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>

using namespace Poco::Net;

class Client
{
  public:
    enum Action : uint32_t
    {
        LOGIN = 1,
        LOGOUT = 2,
        MOVE = 3,
        UPGRADE = 4,
        TURN = 5,
        PLAYER = 6,
        GAMES = 7,
        MAP = 10
    };
    enum Result : uint32_t
    {
        OKEY = 0,
        BAD_COMMAND = 1,
        RESOURCE_NOT_FOUND = 2,
        ACCESS_DENIED = 3,
        INAPPROPRIATE_GAME_STATE = 4,
        TIMEOUT = 5,
        INTERNAL_SERVER_ERROR = 500
    };
    struct ActionMessage
    {
        Action actionCode;
        int dataLength;
        std::string data;
    };
    struct ResponseMessage
    {
        Result result;
        int dataLength;
        std::string data;
    };

  public:
    Client();
    ResponseMessage Login(const std::string &name, const std::string &password = "", const std::string &game = "",
                         int num_turns = -1, int num_players = 1);
    ResponseMessage Player();
    ResponseMessage Logout();
    ResponseMessage Map(int layer);

  private:
    void Send(const ActionMessage &actionMessage);
    ResponseMessage ReceiveResponse();
    StreamSocket ss_;
};

#endif // RAIL_SIMULATOR_CLIENT_H
