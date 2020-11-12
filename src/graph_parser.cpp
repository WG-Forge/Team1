#include "graph_parser.h"

graph GraphParser::GetGraph() const {
  std::string name = document_["name"].GetString();
  size_t id = document_["idx"].GetUint();

  std::vector<graph::Point> points;
  for (const auto &point : document_["points"].GetArray()) {
    if (point["post_idx"].IsUint()) {
      points.emplace_back(point["idx"].GetUint(), point["post_idx"].GetUint());
    } else {
      points.emplace_back(point["idx"].GetUint());
    }
  }

  std::vector<graph::Line> lines;
  for (const auto &line : document_["lines"].GetArray()) {
    lines.emplace_back(
        line["idx"].GetUint(), line["length"].GetDouble(),
        std::make_pair<size_t, size_t>(line["points"][0].GetUint(),
                                       line["points"][1].GetUint()));
  }

  return graph(name, id, points, lines);
}
