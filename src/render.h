#ifndef RAIL_SIMULATOR_RENDER_H
#define RAIL_SIMULATOR_RENDER_H

#include "camera.h"
#include "src/states/state.h"

#include <stack>

class Render
{
  public:
    bool canTarget = true;

    Render() = default;

    void setWindow(sf::RenderWindow *renderWindow);

    void setCamera(Camera *pCamera);

    void draw(State &state);

    void loadFont(const std::string &name, const std::string &path);

    bool isTarget() const;

    bool isPicked(State &state) const;

    void backUp(State &state);

  private:
    sf::RenderWindow *window = nullptr;
    Camera *camera = nullptr;
    std::unordered_map<std::string, sf::Font> font;
    std::stack<GraphState::Point> backups;
    int target = -1;
};

#endif // RAIL_SIMULATOR_RENDER_H