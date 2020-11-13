#ifndef RAIL_SIMULATOR_STATE_H
#define RAIL_SIMULATOR_STATE_H

#include <iostream>
#include <utility>
#include <vector>

#include "SFML/Graphics.hpp"
#include "graph.h"

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
  GraphState(std::vector<Point> points,
             std::vector<std::vector<int>> adjencyList);

  [[nodiscard]] const std::vector<Point> &GetPoints() const;
  [[nodiscard]] const std::vector<std::vector<int>> &GetAdjencyList() const;

  void SetPoints(const std::vector<Point> &points);
  void SetAdjencyList(const std::vector<std::vector<int>> &adjencyList);

private:
  std::vector<Point> points;
  std::vector<std::vector<int>> adjencyList;
};

GraphState CreateCircleGraphStateFromGraph(const Graph &graph);

class State {
private:
    void AddLine(float x1, float y1, float x2, float y2);

    void AddCircle(float x, float y, float r);
public:
    State() = default;

    State(GraphState graphState, std::vector<std::pair<sf::Text, std::string>> texts) : graphState(std::move(
            graphState)), texts(std::move(texts)) {};

    void AddText(float x, float y, const std::string &title,
                 const std::string &fontPath);

    const std::vector<sf::CircleShape> &GetCircles();

    const std::vector<std::vector<sf::Vertex>> &GetLines();

    const std::vector<std::pair<sf::Text, std::string>> &GetTexts();
private:
    float radius = 1.f;
    std::vector<sf::CircleShape> circles;
    std::vector<std::vector<sf::Vertex>> lines;
    std::vector<std::pair<sf::Text, std::string>> texts;
    GraphState graphState;
};

#endif // RAIL_SIMULATOR_STATE_H
