#ifndef RAIL_SIMULATOR_APPLICATION_H
#define RAIL_SIMULATOR_APPLICATION_H

#include <fstream>
#include <queue>
#include <sstream>

#include "camera.h"
#include "config.h"
#include "graph_parser.h"
#include "render.h"

class Application {
private:
    const Config config;
    Camera camera;
    sf::RenderWindow window;
    Render render;
    std::queue<State> states;

    void init();
public:
    explicit Application(Config config);

    int run();
};


#endif //RAIL_SIMULATOR_APPLICATION_H
