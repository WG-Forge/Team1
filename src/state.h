#ifndef RAIL_SIMULATOR_STATE_H
#define RAIL_SIMULATOR_STATE_H

#include <vector>

#include "SFML/Graphics.hpp"
#include "graph.h"

struct State {
  std::vector<sf::CircleShape> circles;
  std::vector<std::vector<sf::Vertex>> lines;
  std::vector<std::pair<sf::Text, std::string>> texts;

  void addLine(int x1, int y1, int x2, int y2) {
    lines.push_back(
        {sf::Vertex(sf::Vector2f(x1, y1)), sf::Vertex(sf::Vector2f(x2, y2))});
  }

  void addCircle(int x, int y, int r) {
    sf::CircleShape circle((float)r);
    circle.setPosition((float)(x - r), (float)(y - r));
    circles.push_back(circle);
  }

  void addText(int x, int y, const std::string &title,
               const std::string &fontPath) {
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

class GraphState {
public:
  struct Point {
    Point &operator=(const Point &rhs);
    bool operator==(const Point &rhs) const;
    bool operator!=(const Point &rhs) const;
    float x, y;
    size_t idx;
  };

public:
  GraphState() = default;
  GraphState(const std::vector<Point> &points,
             const std::vector<std::vector<Point *>> &adjencyList);

  const std::vector<Point> &GetPoints() const;
  const std::vector<std::vector<Point *>> &GetAdjencyList() const;

  void SetPoints(const std::vector<Point> &points);
  void SetAdjencyList(const std::vector<std::vector<Point *>> &adjencyList);

private:
  std::vector<Point> points;
  std::vector<std::vector<Point *>> adjencyList;
};

GraphState CreateCircleGraphStateFromGraph(const Graph &graph);

#endif // RAIL_SIMULATOR_STATE_H
