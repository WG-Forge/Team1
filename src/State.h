#ifndef RAIL_SIMULATOR_STATE_H
#define RAIL_SIMULATOR_STATE_H

#include <vector>

#include "SFML/Graphics.hpp"

struct State {
    std::vector<sf::CircleShape> circles;
    std::vector<std::vector<sf::Vertex>> lines;

    void addLine(int x1, int y1, int x2, int y2) {
        lines.push_back({sf::Vertex(sf::Vector2f(x1, y1)), sf::Vertex(sf::Vector2f(x2, y2))});
    }

    void addCircle(int x, int y, int r) {
        sf::CircleShape circle((float)r);
        circle.setPosition((float)x - r, (float)y - r);
        circles.push_back(circle);
    }

    State() = default;
};


#endif //RAIL_SIMULATOR_STATE_H
