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

    void SetWindow(sf::RenderWindow *renderWindow);
    void SetCamera(Camera *pCamera);
    void Draw(State &state);
    void LoadFont(const std::string &name, const std::string &path);
    void LoadTexture(const std::string &name, const std::string &path);
    bool IsTarget() const;
    bool IsPicked(State &state) const;
    void BackUp(State &state);
    int GetPicked(State &state) const;

  private:
    sf::RenderWindow *window = nullptr;
    Camera *camera = nullptr;
    std::unordered_map<std::string, sf::Font> font;
    std::unordered_map<std::string, sf::Texture> texture;
    std::stack<RailGraph::Graph::Point> backups;
    int target = -1;
};

#endif // RAIL_SIMULATOR_RENDER_H