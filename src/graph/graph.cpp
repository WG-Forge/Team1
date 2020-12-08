#include "graph.h"

#include <utility>

namespace RailGraph
{
Graph::Graph(std::string name, size_t id, const std::vector<Point> &points, std::vector<Line> lines)
    : name(std::move(name)), idx(id), points(points), lines(std::move(lines))
{
    for (size_t i = 0; i < points.size(); ++i)
    {
        idxToNum[points[i].idx] = i;
    }
}

void Graph::SetName(const std::string &name_)
{
    this->name = name_;
}

void Graph::SetId(size_t id)
{
    idx = id;
}

void Graph::SetPointsCoordinates(const std::vector<std::tuple<size_t, int, int>> &coordinates)
{
    for (const auto & coordinate : coordinates) {
        points[idxToNum[std::get<0>(coordinate)]].x = std::get<1>(coordinate);
        points[idxToNum[std::get<0>(coordinate)]].y = std::get<2>(coordinate);
    }
}

void Graph::SetLines(const std::vector<Line> &lines_)
{
    lines = lines_;
}

void Graph::SetPosts(const std::vector<Post> &posts)
{
    this->posts = posts;
    for (size_t i = 0; i < posts.size(); ++i)
    {
        pointToPost[posts[i].info.point_idx] = i;
    }
}

const std::string &Graph::GetName() const
{
    return name;
}
size_t Graph::GetIdx() const
{
    return idx;
}
const std::vector<Graph::Point> &Graph::GetPoints() const
{
    return points;
}
const std::vector<Graph::Line> &Graph::GetLines() const
{
    return lines;
}
size_t Graph::GetNum(size_t pointIdx) const
{
    return idxToNum.at(pointIdx);
}

Graph::Point::Point(size_t idx_, std::optional<size_t> postIdx_) : postIdx(std::move(postIdx_)), idx(idx_)
{
}

bool Graph::Point::operator==(const Graph::Point &rhs) const
{
    return idx == rhs.idx && postIdx == rhs.postIdx;
}

bool Graph::Point::operator!=(const Graph::Point &rhs) const
{
    return !(rhs == *this);
}

Graph::Post::Post(const Graph::Info &info, const std::variant<MarketInfo, CityInfo, StorageInfo> &postInfo)
    : info(info), postInfo(postInfo)
{
}

Graph::Line::Line(size_t idx, double length, std::pair<size_t, size_t> points)
    : idx(idx), length(length), points(std::move(points))
{
}

bool Graph::Line::operator==(const Graph::Line &rhs) const
{
    return idx == rhs.idx && length == rhs.length && points == rhs.points;
}

bool Graph::Line::operator!=(const Graph::Line &rhs) const
{
    return !(rhs == *this);
}
} // namespace RailGraph