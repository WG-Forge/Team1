#include "graph_parser.h"

namespace RailGraph
{
Graph GraphParser::GetGraph() const
{
    std::string name = document["name"].GetString();
    size_t id = document["idx"].GetUint();

    std::vector<Graph::Point> points;
    for (const auto &point : document["points"].GetArray())
    {
        if (point["post_idx"].IsUint())
        {
            points.emplace_back(point["idx"].GetUint(), point["post_idx"].GetUint());
        }
        else
        {
            points.emplace_back(point["idx"].GetUint());
        }
    }

    std::vector<Graph::Line> lines;
    for (const auto &line : document["lines"].GetArray())
    {
        lines.emplace_back(line["idx"].GetUint(), line["length"].GetDouble(),
                           std::make_pair<size_t, size_t>(line["points"][0].GetUint(), line["points"][1].GetUint()));
    }

    return Graph(name, id, points, lines);
}
} // namespace RailGraph
