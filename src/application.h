#ifndef RAIL_SIMULATOR_APPLICATION_H
#define RAIL_SIMULATOR_APPLICATION_H

#include <fstream>
#include <queue>
#include <sstream>

#include "camera.h"
#include "client.h"
#include "config.h"
#include "graph_parser.h"
#include "render.h"

class Application
{
  private:
    const Config config;
    Camera camera;
    Client client;
    sf::RenderWindow window;
    Render render;
    std::queue<State> states;
    std::unordered_map<sf::Keyboard::Key, bool> touched;
    bool focusedConsole = true;
    std::string consoleHistory;

    void init();

    void PollCommand(std::string);

  public:
    Application(Config config, io_service &ioService);

    int run();
};

#endif // RAIL_SIMULATOR_APPLICATION_H
