#include "graph.h"

namespace RailGraph {
Graph::Graph(const std::string &name, size_t id,
             const std::vector<Point> &points, const std::vector<Line> &lines)
    : name(name), idx(id), points(points), lines(lines) {
  for (size_t i = 0; i < points.size(); ++i) {
    idxToNum[points[i].idx] = i;
  }
}

void Graph::SetName(const std::string &name) { this->name = name; }

void Graph::SetId(size_t id) { idx = id; }
void Graph::SetPoints(const std::vector<Point> &points) {
  this->points = points;
}
void Graph::SetLines(const std::vector<Line> &lines) { this->lines = lines; }

const std::string &Graph::GetName() const { return name; }
size_t Graph::GetIdx() const { return idx; }
const std::vector<Graph::Point> &Graph::GetPoints() const { return points; }
const std::vector<Graph::Line> &Graph::GetLines() const { return lines; }
size_t Graph::GetNum(size_t pointIdx) const { return idxToNum.at(pointIdx); }

Graph::Point::Point(size_t idx, std::optional<size_t> postIdx)
    : postIdx(postIdx), idx(idx) {}

bool Graph::Point::operator==(const Graph::Point &rhs) const {
  return idx == rhs.idx && postIdx == rhs.postIdx;
}

bool Graph::Point::operator!=(const Graph::Point &rhs) const {
  return !(rhs == *this);
}

Graph::Line::Line(size_t idx, double length,
                  const std::pair<size_t, size_t> &points)
    : idx(idx), length(length), points(points) {}

bool Graph::Line::operator==(const Graph::Line &rhs) const {
  return idx == rhs.idx && length == rhs.length && points == rhs.points;
}

bool Graph::Line::operator!=(const Graph::Line &rhs) const {
  return !(rhs == *this);
}
} // namespace RailGraph