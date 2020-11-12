#ifndef RAIL_SIMULATOR_GRAPH_H
#define RAIL_SIMULATOR_GRAPH_H
#include <unordered_map>
#include <vector>

class graph {
public:
  struct Point {
    explicit Point(size_t idx, std::optional<size_t> postIdx = std::nullopt);
    bool operator==(const Point &rhs) const;
    bool operator!=(const Point &rhs) const;
    size_t idx;
    std::optional<size_t> postIdx;
  };

  struct Line {
    Line(size_t idx, double length, const std::pair<size_t, size_t> &points);
    bool operator==(const Line &rhs) const;
    bool operator!=(const Line &rhs) const;
    size_t idx;
    double length;
    std::pair<size_t, size_t> points;
  };

public:
  graph(const std::string &name, size_t id, const std::vector<Point> &points,
        const std::vector<Line> &lines);

  void SetName(const std::string &name);
  void SetId(size_t id);
  void SetPoints(const std::vector<Point> &points);
  void SetLines(const std::vector<Line> &lines);

  const std::string &GetName() const;
  size_t GetIdx() const;
  const std::vector<Point> &GetPoints() const;
  const std::vector<Line> &GetLines() const;

private:
  std::string name_;
  size_t idx_;
  std::vector<Point> points_;
  std::vector<Line> lines_;
};

#endif
