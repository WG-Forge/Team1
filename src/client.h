#ifndef RAIL_SIMULATOR_CLIENT_H
#define RAIL_SIMULATOR_CLIENT_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include <boost/asio.hpp>

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
    explicit Client(io_service &io_service);
    ResponseMessage Login(const std::string &name, const std::string &password = "", const std::string &game = "",
                          int num_turns = -1, int num_players = 1);
    ResponseMessage Logout();
    ResponseMessage Player();
    ResponseMessage Map(int layer);
    ResponseMessage Move(size_t line_idx, int speed, size_t train_idx);
    ResponseMessage Upgrade(const std::vector<size_t> &posts, const std::vector<size_t> &trains);
    ResponseMessage Turn();
    ResponseMessage Games();
    ip::tcp::socket &GetSocket();
  private:
    void Send(const ActionMessage &actionMessage);
    ResponseMessage ReceiveResponse();
    ip::tcp::socket socket_;
};

#endif // RAIL_SIMULATOR_CLIENT_H
