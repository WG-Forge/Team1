#ifndef RAIL_SIMULATOR_STATE_H
#define RAIL_SIMULATOR_STATE_H

#include <vector>

#include "SFML/Graphics.hpp"

struct State {
    std::vector<sf::CircleShape> circles;
    std::vector<std::vector<sf::Vertex>> lines;
    std::vector<std::pair<sf::Text, std::string>> texts;

    void addLine(int x1, int y1, int x2, int y2) {
        lines.push_back({sf::Vertex(sf::Vector2f(x1, y1)), sf::Vertex(sf::Vector2f(x2, y2))});
    }

    void addCircle(int x, int y, int r) {
        sf::CircleShape circle((float)r);
        circle.setPosition((float)(x - r), (float)(y - r));
        circles.push_back(circle);
    }

    void addText(int x, int y, const std::string& title, const std::string& fontPath) {
        sf::Text text;
        text.setString(title);
        text.setPosition((float)x, (float)y);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);
        texts.emplace_back(text, fontPath);
    }

    void resize(int delta) {
        for (auto &circle : circles) {
            circle.setPosition(circle.getPosition().x + (float)delta,
                               circle.getPosition().y + (float)delta);
        }
        for (auto &l : lines) {
            l[0].position.x += (float)delta;
            l[1].position.x += (float)delta;
            l[0].position.y += (float)delta;
            l[1].position.y += (float)delta;
        }
    }

    State() = default;
};


#endif //RAIL_SIMULATOR_STATE_H
