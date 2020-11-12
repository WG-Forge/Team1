#include "graph.h"
graph::graph(const std::string &name, size_t id,
             const std::vector<Point> &points, const std::vector<Line> &lines)
    : name_(name), idx_(id), points_(points), lines_(lines) {}

void graph::SetName(const std::string &name) { name_ = name; }

void graph::SetId(size_t id) { idx_ = id; }
void graph::SetPoints(const std::vector<Point> &points) { points_ = points; }
void graph::SetLines(const std::vector<Line> &lines) { lines_ = lines; }

const std::string &graph::GetName() const { return name_; }
size_t graph::GetIdx() const { return idx_; }
const std::vector<graph::Point> &graph::GetPoints() const { return points_; }
const std::vector<graph::Line> &graph::GetLines() const { return lines_; }

graph::Point::Point(size_t idx, std::optional<size_t> postIdx)
    : postIdx(postIdx), idx(idx) {}

bool graph::Point::operator==(const graph::Point &rhs) const {
  return idx == rhs.idx && postIdx == rhs.postIdx;
}

bool graph::Point::operator!=(const graph::Point &rhs) const {
  return !(rhs == *this);
}

graph::Line::Line(size_t idx, double length,
                  const std::pair<size_t, size_t> &points)
    : idx(idx), length(length), points(points) {}

bool graph::Line::operator==(const graph::Line &rhs) const {
  return idx == rhs.idx && length == rhs.length && points == rhs.points;
}

bool graph::Line::operator!=(const graph::Line &rhs) const {
  return !(rhs == *this);
}
