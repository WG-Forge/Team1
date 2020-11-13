#ifndef RAIL_SIMULATOR_RENDER_H
#define RAIL_SIMULATOR_RENDER_H

#include "camera.h"
#include "state.h"

class Render {
private:
    sf::RenderWindow *window = nullptr;
    Camera *camera = nullptr;
    std::unordered_map<std::string, sf::Font> font;
    bool hide = true;
public:
    Render() = default;

    void setWindow(sf::RenderWindow *renderWindow);

    void setCamera(Camera *pCamera);

    void draw(State& state);

    void loadFont(const std::string& name, const std::string& path);

    void hideInformationSwitch () { hide ^= true; }
};


#endif //RAIL_SIMULATOR_RENDER_H
