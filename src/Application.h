#ifndef RAIL_SIMULATOR_APPLICATION_H
#define RAIL_SIMULATOR_APPLICATION_H

#include <iostream>
#include <queue>

#include "Camera.h"
#include "Config.h"
#include "Render.h"
#include "State.h"

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
