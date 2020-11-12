#include "state.h"
#include <math.h>
#include <vector>

GraphState::GraphState(
    const std::vector<GraphState::Point> &points,
    const std::vector<std::vector<GraphState::Point *>> &adjencyList)
    : points(points), adjencyList(adjencyList) {}

bool GraphState::Point::operator==(const GraphState::Point &rhs) const {
  return x == rhs.x && y == rhs.y && idx == rhs.idx;
}

bool GraphState::Point::operator!=(const GraphState::Point &rhs) const {
  return !(rhs == *this);
}

GraphState::Point &
GraphState::Point::operator=(const GraphState::Point &rhs) {
  x = rhs.x, y = rhs.y;
  idx = rhs.idx;
  return *this;
}

const std::vector<GraphState::Point> &GraphState::GetPoints() const {
  return points;
}

const std::vector<std::vector<GraphState::Point *>> &
GraphState::GetAdjencyList() const {
  return adjencyList;
}

void GraphState::SetPoints(const std::vector<Point> &points) {
  GraphState::points = points;
}

void GraphState::SetAdjencyList(
    const std::vector<std::vector<Point *>> &adjencyList) {
  GraphState::adjencyList = adjencyList;
}

GraphState CreateCircleGraphStateFromGraph(const Graph &graph) {
  std::vector<GraphState::Point> vertices(graph.GetPoints().size());
  std::vector<std::vector<GraphState::Point *>> adjencyList(vertices.size());
  for (int i = 0; i < vertices.size(); ++i) {
    vertices[i] = {cosf(i * M_PI / vertices.size()),
                   sinf(i * M_PI / vertices.size()), graph.GetPoints()[i].idx};
  }
  for (const auto &line : graph.GetLines()) {
    size_t firstNum = graph.GetNum(line.points.first),
           secondNum = graph.GetNum(line.points.second);
    adjencyList[firstNum].emplace_back(&vertices[secondNum]);
    adjencyList[secondNum].emplace_back(&vertices[firstNum]);
  }
  return GraphState(vertices, adjencyList);
}
