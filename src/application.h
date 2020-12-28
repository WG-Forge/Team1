#ifndef RAIL_SIMULATOR_APPLICATION_H
#define RAIL_SIMULATOR_APPLICATION_H

#include <fstream>
#include <mutex>
#include <queue>
#include <sstream>

#include "camera.h"
#include "config.h"
#include "render.h"
#include "src/client/client.h"
#include "src/game/brain.h"
#include "src/util/parser.h"

class Application
{
  private:
    const Config config;
    Camera camera;
    sf::RenderWindow window;
    Render render;
    State state;
    RailGraph::Graph map;
    Brain brain;
    std::mutex stateMutex;
    static const int clientsNum = 10;
    std::mutex clientMutexes[clientsNum];
    Client clients[clientsNum];
    bool isSoloGame{false};

    void Init();
    void HandleCommand(std::string, bool, int);
    void PollEvent(sf::Event &);

  public:
    Application(Config config);
    int Run();

  private:
    std::unordered_map<sf::Keyboard::Key, bool> touched;
    bool focusedConsole = true, firstRender = true, pause = false;
    std::string consoleHistory, consoleInformation;
    char console[255] = "";
    sf::Clock deltaClock, dtTimer;
    int mouseX = -1, mouseY = -1, cameraX = -1, cameraY = -1;
    float delta = 0;
};

#endif // RAIL_SIMULATOR_APPLICATION_H
