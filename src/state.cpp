#include "state.h"
#include <math.h>
#include <utility>
#include <vector>

GraphState::GraphState(std::vector<GraphState::Point> points,
                       std::vector<std::vector<int>> adjencyList)
    : points(std::move(points)), adjencyList(std::move(adjencyList)) {}

bool GraphState::Point::operator==(const GraphState::Point &rhs) const {
  return x == rhs.x && y == rhs.y && idx == rhs.idx;
}

bool GraphState::Point::operator!=(const GraphState::Point &rhs) const {
  return !(rhs == *this);
}

GraphState::Point &GraphState::Point::operator=(const GraphState::Point &rhs) {
  x = rhs.x, y = rhs.y;
  idx = rhs.idx;
  return *this;
}

const std::vector<GraphState::Point> &GraphState::GetPoints() const {
  return points;
}

const std::vector<std::vector<int>> &GraphState::GetAdjencyList() const {
  return adjencyList;
}

void GraphState::SetPoints(const std::vector<Point> &points) {
  GraphState::points = points;
}

void GraphState::SetAdjencyList(
    const std::vector<std::vector<int>> &adjencyList) {
  GraphState::adjencyList = adjencyList;
}

GraphState CreateCircleGraphStateFromGraph(const Graph &graph) {
  std::vector<GraphState::Point> vertices(graph.GetPoints().size());
  std::vector<std::vector<int>> adjencyList(vertices.size());
  for (int i = 0; i < vertices.size(); ++i) {
    vertices[i] = {cosf(i * 2 * M_PI / vertices.size()) * 1000,
                   sinf(i * 2 * M_PI / vertices.size()) * 1000,
                   graph.GetPoints()[i].idx};
  }
  for (const auto &line : graph.GetLines()) {
    size_t firstNum = graph.GetNum(line.points.first),
           secondNum = graph.GetNum(line.points.second);
    adjencyList[firstNum].emplace_back(secondNum);
    adjencyList[secondNum].emplace_back(firstNum);
  }
  return GraphState(std::move(vertices), std::move(adjencyList));
}

void State::AddLine(float x1, float y1, float x2, float y2) {
  lines.push_back(
      {sf::Vertex(sf::Vector2f(x1, y1)), sf::Vertex(sf::Vector2f(x2, y2))});
}

void State::AddCircle(float x, float y, float r) {
  sf::CircleShape circle(r);
  circle.setPosition(x - r, y - r);
  circles.push_back(circle);
}

void State::AddText(float x, float y, const std::string &title,
                    const std::string &fontPath) {
  sf::Text text;
  text.setString(title);
  text.setPosition(x, y);
  text.setCharacterSize(14);
  text.setFillColor(sf::Color::White);
  texts.emplace_back(text, fontPath);
}

const std::vector<sf::CircleShape> &State::GetCircles() {
  State::circles.clear();
  for (const auto &point : State::graphState.GetPoints()) {
    State::AddCircle(point.x, point.y, radius);
  }
  return State::circles;
}

const std::vector<std::vector<sf::Vertex>> &State::GetLines() {
  State::lines.clear();
  auto points = State::graphState.GetPoints();
  auto edges = State::graphState.GetAdjencyList();
  for (size_t i = 0; i < points.size(); ++i) {
    for (const auto &v : edges[i]) {
      State::AddLine(points[i].x, points[i].y, points[v].x, points[v].y);
    }
  }
  return State::lines;
}

const std::vector<std::pair<sf::Text, std::string>> &State::GetTexts() {
  return State::texts;
}
