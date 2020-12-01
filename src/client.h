#ifndef RAIL_SIMULATOR_CLIENT_H
#define RAIL_SIMULATOR_CLIENT_H

#include <boost/asio.hpp>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using namespace boost::asio;

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
    explicit Client(io_service& io_service);
    ResponseMessage Login(const std::string &name, const std::string &password = "", const std::string &game = "",
                         int num_turns = -1, int num_players = 1);
    ResponseMessage Player();
    ResponseMessage Logout();
    ResponseMessage Map(int layer);

  private:
    void Send(const ActionMessage &actionMessage);
    ResponseMessage ReceiveResponse();
    ip::tcp::socket socket_;
};

#endif // RAIL_SIMULATOR_CLIENT_H
