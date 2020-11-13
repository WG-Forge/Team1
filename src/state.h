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
    float x = 0, y = 0;
    size_t idx{};
    Point(float x_, float y_, size_t idx_) : x(x_), y(y_), idx(idx_) {};
    Point() = default;
  };

public:
  GraphState() = default;
  GraphState(std::vector<Point> points,
             std::vector<std::vector<int>> adjencyList);

  [[nodiscard]] std::vector<Point> &GetPoints();
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

    State(GraphState graphState, std::vector<std::pair<sf::Text, std::string>> information);

    void AddInformation(float x, float y, const std::string &title,
                        const std::string &fontName, int size, sf::Color color);

    const std::vector<std::pair<sf::Text, std::string>> &GetInformation();

    const std::vector<sf::CircleShape> &GetCircles();

    const std::vector<std::vector<sf::Vertex>> &GetLines();

    void AddText(float x, float y, const std::string &title,
                 const std::string &fontName, int size, sf::Color color);

    const std::vector<std::pair<sf::Text, std::string>> &GetTexts();

    void Resize(float X, float Y, float delta);

    GraphState::Point GetCenter();

    static float GetLen (GraphState::Point point1, GraphState::Point point2);
private:
    float radius = 1.f;
    std::vector<sf::CircleShape> circles;
    std::vector<std::vector<sf::Vertex>> lines;
    std::vector<std::pair<sf::Text, std::string>> texts;
    std::vector<std::pair<sf::Text, std::string>> pointInformation;
    GraphState graphState;
};

#endif // RAIL_SIMULATOR_STATE_H