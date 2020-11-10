#include "graph.h"
Graph::Graph(const std::string &name, size_t id,
             const std::vector<Point> &points, const std::vector<Line> &lines)
    : name_(name), idx_(id), points_(points), lines_(lines) {}

void Graph::SetName(const std::string &name) { name_ = name; }

void Graph::SetId(size_t id) { idx_ = id; }
void Graph::SetPoints(const std::vector<Point> &points) { points_ = points; }
void Graph::SetLines(const std::vector<Line> &lines) { lines_ = lines; }

const std::string &Graph::GetName() const { return name_; }
size_t Graph::GetIdx() const { return idx_; }
const std::vector<Graph::Point> &Graph::GetPoints() const { return points_; }
const std::vector<Graph::Line> &Graph::GetLines() const { return lines_; }

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
