#include "graph.h"

#include <iostream>
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

int Graph::Post::MarketIndex = 0;
int Graph::Post::CityIndex = 1;
int Graph::Post::StorageIndex = 2;

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
    for (const auto &coordinate : coordinates)
    {
        points[idxToNum[std::get<0>(coordinate)]].renderX = points[idxToNum[std::get<0>(coordinate)]].x =
            std::get<1>(coordinate);
        points[idxToNum[std::get<0>(coordinate)]].renderY = points[idxToNum[std::get<0>(coordinate)]].y =
            std::get<2>(coordinate);
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

void Graph::SetTrains(const std::vector<Train> &trains_)
{
    trains = trains_;
}

const std::string &Graph::GetName() const
{
    return name;
}
size_t Graph::GetIdx() const
{
    return idx;
}
std::vector<Graph::Point> &Graph::GetPoints()
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
float Graph::GetDist(const Graph::Point &first, const Graph::Point &second)
{
    return sqrtf((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y));
}
float Graph::GetRenderDist(const Graph::Point &first, const Graph::Point &second)
{
    return sqrtf((first.renderX - second.renderX) * (first.renderX - second.renderX) +
                 (first.renderY - second.renderY) * (first.renderY - second.renderY));
}
std::vector<RailGraph::Graph::Post> Graph::GetPosts() const
{
    return posts;
}
std::vector<Graph::Train> Graph::GetTrains() const
{
    return trains;
}
std::string Graph::GetPointInfo(int index) const
{
    std::optional<std::string> info(std::nullopt);
    for (const auto &post : posts)
    {
        if (post.info.point_idx == index)
        {
            std::string result = "{\n";
            result.append("    name: " + post.info.name + "\n");
            result.append("    idx: " + std::to_string(post.info.idx) + "\n");
            result.append("    pointIdx: " + std::to_string(post.info.point_idx) + "\n");
            switch (post.postInfo.index())
            {
            case 0: {
                auto market = std::get<MarketInfo>(post.postInfo);
                result.append("    type: market\n");
                result.append("    product: " + std::to_string(market.product) + "\n");
                result.append("    productCapacity: " + std::to_string(market.productCapacity) + "\n");
                result.append("    replenishment: " + std::to_string(market.replenishment) + "\n");
                break;
            }
            case 1: {
                auto city = std::get<CityInfo>(post.postInfo);
                result.append("    type: city\n");
                result.append("    armor: " + std::to_string(city.armor) + "\n");
                result.append("    armorCapacity: " + std::to_string(city.armorCapacity) + "\n");
                result.append("    level: " + std::to_string(city.level) + "\n");
                result.append(
                    "    nextLevelPrice: " +
                    (city.nextLevelPrice == std::nullopt ? "null" : std::to_string(city.nextLevelPrice.value())) +
                    "\n");
                result.append("    population: " + std::to_string(city.population) + "\n");
                result.append("    populationCapacity: " + std::to_string(city.populationCapacity) + "\n");
                result.append("    product: " + std::to_string(city.product) + "\n");
                result.append("    productCapacity: " + std::to_string(city.productCapacity) + "\n");
                result.append("    trainCooldown: " + std::to_string(city.trainCooldown) + "\n");
                result.append("    playerIdx: " + (city.playerIdx == std::nullopt ? "null" : city.playerIdx.value()) +
                              "\n");
                break;
            }
            case 2: {
                auto storage = std::get<StorageInfo>(post.postInfo);
                result.append("    type: storage\n");
                result.append("    armor: " + std::to_string(storage.armor) + "\n");
                result.append("    armorCapacity: " + std::to_string(storage.armorCapacity) + "\n");
                result.append("    replenishment: " + std::to_string(storage.replenishment) + "\n");
                break;
            }
            default: {
                break;
            }
            }
            result.append("}");
            info = result;
            break;
        }
    }
    return (info == std::nullopt ? "no such post '" + std::to_string(index) + "'" : info.value());
}
std::string Graph::GetTrainInfo(int index) const
{
    std::optional<std::string> info(std::nullopt);
    for (const auto &train : trains)
    {
        if (train.info.idx == index)
        {
            auto trainInfo = train.info;
            std::string result = "{\n";
            result.append("    type: " + std::to_string(trainInfo.idx) + "\n");
            result.append("    cooldown: " + std::to_string(trainInfo.cooldown) + "\n");
            result.append("    fuel: " + std::to_string(trainInfo.fuel) + "\n");
            result.append("    fuelCapacity: " + std::to_string(trainInfo.fuelCapacity) + "\n");
            result.append("    fuelConsumption: " + std::to_string(trainInfo.fuelConsumption) + "\n");
            result.append("    goods: " + std::to_string(trainInfo.goods) + "\n");
            result.append("    goodsCapacity: " + std::to_string(trainInfo.goodsCapacity) + "\n");
            result.append("    goodsType: " +
                          (trainInfo.goodsType == std::nullopt ? "null" : std::to_string(trainInfo.goodsType.value())) +
                          "\n");
            result.append("    idx: " + std::to_string(trainInfo.idx) + "\n");
            result.append("    lineIdx: " + std::to_string(trainInfo.lineIdx) + "\n");
            result.append("    level: " + std::to_string(trainInfo.level) + "\n");
            result.append("    nextLevelPrice: " + std::to_string(trainInfo.nextLevelPrice) + "\n");
            result.append("    playerIdx: " +
                          (trainInfo.playerIdx == std::nullopt ? "null" : trainInfo.playerIdx.value()) + "\n");
            result.append("    position: " + std::to_string(trainInfo.position) + "\n");
            result.append("    speed: " + std::to_string(trainInfo.speed) + "\n");
            result.append("}");
            info = result;
            break;
        }
    }
    return (info == std::nullopt ? "no such train '" + std::to_string(index) + "'" : info.value());
}

std::vector<RailGraph::Graph::Rating> Graph::GetRatings() const
{
    return Graph::ratings;
}

void Graph::SetRatings(const std::vector<Rating> &ratings)
{
    Graph::ratings = ratings;
}

Graph::Point::Point(size_t idx_, std::optional<size_t> postIdx_) : postIdx(std::move(postIdx_)), idx(idx_)
{
}

Graph::Point::Point(float x, float y, float renderX, float renderY) : x(x), y(y), renderX(renderX), renderY(renderY)
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

Graph::Post::Post(Graph::Info info, std::variant<MarketInfo, CityInfo, StorageInfo> postInfo)
    : info(std::move(info)), postInfo(std::move(postInfo))
{
}
Graph::Train::Train(Graph::TrainInfo info) : info(std::move(info))
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