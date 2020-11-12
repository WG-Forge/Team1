#ifndef RAIL_SIMULATOR_RENDER_H
#define RAIL_SIMULATOR_RENDER_H

#include "camera.h"
#include "state.h"

class Render {
private:
    sf::RenderWindow *window = nullptr;
    Camera *camera = nullptr;
public:
    Render() = default;

    void setWindow(sf::RenderWindow *renderWindow);

    void setCamera(Camera *pCamera);

    void draw(State& state);
};


#endif //RAIL_SIMULATOR_RENDER_H
