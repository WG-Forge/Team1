#ifndef RAIL_SIMULATOR_STATE_H
#define RAIL_SIMULATOR_STATE_H

#include "SFML/Graphics.hpp"
#include "graph_state.h"
#include "src/util/sfml_tool.h"

class State
{
  private:
    void AddLine(const std::vector<sf::Vertex> &);
    void AddCircle(const sf::CircleShape &);
    void AddStaticText(const sf::Text &, const std::string &);
    void AddNonStaticText(const sf::Text &, const std::string &);

  public:
    State() = default;
    State(GraphState graphState, std::vector<std::pair<sf::Text, std::string>> information);

    std::vector<std::pair<sf::CircleShape, std::string>> GetCircles();
    const std::vector<std::vector<sf::Vertex>> &GetLines();
    std::vector<std::pair<sf::Text, std::string>> GetNonStaticTexts();
    std::vector<std::pair<sf::Text, std::string>> GetStaticTexts();

    GraphState::Point GetCenter();
    void Resize(float delta);
    void ChangePointLocation(int index, float X, float Y);
    void ResetPointInformation();

  private:
    float circleRadius = 1.f;
    bool showPointInformation = false;

    std::vector<sf::CircleShape> circles;
    std::vector<std::vector<sf::Vertex>> lines;
    std::vector<std::pair<sf::Text, std::string>> nonStaticTexts, staticTexts;

    GraphState graphState;
};

#endif // RAIL_SIMULATOR_STATE_H
