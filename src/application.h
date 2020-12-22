#ifndef RAIL_SIMULATOR_APPLICATION_H
#define RAIL_SIMULATOR_APPLICATION_H

#include <fstream>
#include <queue>
#include <sstream>

#include "camera.h"
#include "config.h"
#include "render.h"
#include "src/client/client.h"
#include "src/util/parser.h"
#include "src/game/brain.h"

class Application
{
  private:
    const Config config;
    Camera camera;
    Client client;
    sf::RenderWindow window;
    Render render;
    State state;
    RailGraph::Graph map;
    Brain brain;

    void Init();
    void HandleCommand(std::string);
    void PollEvent(sf::Event &);

  public:
    Application(Config config, io_service &ioService);
    int Run();

  private:
    std::unordered_map<sf::Keyboard::Key, bool> touched;
    bool focusedConsole = true, firstRender = true;
    std::string consoleHistory;
    char console[255] = "";
    sf::Clock deltaClock, dtTimer;
    int mouseX = -1, mouseY = -1, cameraX = -1, cameraY = -1;
    float delta = 0;
};

#endif // RAIL_SIMULATOR_APPLICATION_H
