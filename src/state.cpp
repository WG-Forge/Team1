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

std::vector<GraphState::Point> &GraphState::GetPoints() {
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
    vertices[i] = {cosf(i * 2 * M_PI / vertices.size()) * 100,
                   sinf(i * 2 * M_PI / vertices.size()) * 100,
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

State::State(GraphState graphState, std::vector<std::pair<sf::Text, std::string>> texts) : graphState(std::move(
        graphState)), texts(std::move(texts)) {
    Resize(0, 0, 0);
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

void State::AddText(float x, float y, const std::string &title,
                    const std::string &fontName, int size, sf::Color color) {
    sf::Text text;
    text.setString(title);
    text.setPosition(x, y);
    text.setCharacterSize(size);
    text.setFillColor(color);
    texts.emplace_back(text, fontName);
}

const std::vector<std::pair<sf::Text, std::string>> &State::GetTexts() {
    return State::texts;
}

void State::AddInformation(float x, float y, const std::string &title,
                           const std::string &fontName, int size, sf::Color color) {
    sf::Text text;
    text.setString(title);
    text.setPosition(x, y);
    text.setCharacterSize(size);
    text.setFillColor(color);
    pointInformation.emplace_back(text, fontName);
}

const std::vector<std::pair<sf::Text, std::string>> &State::GetInformation() {
    pointInformation.clear();
    for (size_t i = 0; i < State::graphState.GetPoints().size(); ++i) {
        AddInformation(circles[i].getPosition().x + circles[i].getRadius() * 2,
                circles[i].getPosition().y + circles[i].getRadius() * 2,
                std::to_string(State::graphState.GetPoints()[i].idx),
                "8-bit-pusab", 10, sf::Color::White);
    }
    return State::pointInformation;
}

float State::GetLen (GraphState::Point point1, GraphState::Point point2) {
    return sqrt((point1.x - point2.x) * (point1.x - point2.x) +
                (point1.y - point2.y) * (point1.y - point2.y));
}

void State::Resize(float X, float Y, float delta) {
    float len = (State::graphState.GetPoints().empty() ? 0 : GetLen(State::graphState.GetPoints()[0], GraphState::Point()));
    for (auto &point : State::graphState.GetPoints()) {
        if (len < 100 && delta < 0) {
            continue;
        }
        point.x += point.x / len * delta;
        point.y += point.y / len * delta;
    }
    float min = 30.f;
    for (const auto &point1 : State::graphState.GetPoints()) {
        for (const auto &point2 : State::graphState.GetPoints()) {
            if (point1 != point2) {
                min = std::min(GetLen(point1, point2), min);
            }
        }
    }
    State::radius = std::max(1.f, min / 3);
}

GraphState::Point State::GetCenter() {
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    for (const auto &point : graphState.GetPoints()) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }
    return GraphState::Point((minX + maxX) / 2.f, (minY + maxY) / 2.f, 0);
}