#ifndef RAIL_SIMULATOR_SFML_TOOL_H
#define RAIL_SIMULATOR_SFML_TOOL_H

#include <iostream>
#include <utility>
#include <vector>

#include "SFML/Graphics.hpp"

class SfmlTool
{
  public:
    static std::vector<sf::Vertex> GetLine(float x1, float y1, float x2, float y2)
    {
        return {sf::Vertex(sf::Vector2f(x1, y1)), sf::Vertex(sf::Vector2f(x2, y2))};
    }
    static sf::CircleShape GetCircle(float x, float y, float r)
    {
        sf::CircleShape circle(r);
        circle.setPosition(x - r, y - r);
        return circle;
    }
    static sf::Text GetText(float x, float y, const std::string &title, int size, sf::Color color)
    {
        sf::Text text;
        text.setString(title);
        text.setPosition(x, y);
        text.setCharacterSize(size);
        text.setFillColor(color);
        return text;
    }
    static void SetTextureOnCircleShape(sf::CircleShape &circleShape, sf::Texture* texture)
    {
        circleShape.setTexture(texture, false);
    }
};

#endif // RAIL_SIMULATOR_SFML_TOOL_H
