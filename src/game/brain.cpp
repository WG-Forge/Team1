#include "brain.h"
#include <SFML/System/Clock.hpp>

Brain::Brain()
{
    townLevelInformation = {{{"Population Capacity", 1}, std::optional<int>(10)},
                            {{"Product Capacity", 1}, std::optional<int>(200)},
                            {{"Armor Capacity", 1}, std::optional<int>(200)},
                            {{"Cooldown After Crash", 1}, std::optional<int>(2)},
                            {{"Next Level Price", 1}, std::optional<int>(100)},
                            {{"Population Capacity", 2}, std::optional<int>(20)},
                            {{"Product Capacity", 2}, std::optional<int>(500)},
                            {{"Armor Capacity", 2}, std::optional<int>(500)},
                            {{"Cooldown After Crash", 2}, std::optional<int>(1)},
                            {{"Next Level Price", 2}, std::optional<int>(200)},
                            {{"Population Capacity", 3}, std::optional<int>(40)},
                            {{"Product Capacity", 3}, std::optional<int>(10000)},
                            {{"Armor Capacity", 3}, std::optional<int>(10000)},
                            {{"Cooldown After Crash", 3}, std::optional<int>(0)},
                            {{"Next Level Price", 3}, std::optional<int>(std::nullopt)}};
    trainLevelInformation = {
        {{"Goods Capacity", 1}, std::optional<int>(40)},  {{"Fuel Capacity", 1}, std::optional<int>(400)},
        {{"Fuel Consumption", 1}, std::optional<int>(1)}, {{"Next Level Price", 1}, std::optional<int>(40)},
        {{"Goods Capacity", 2}, std::optional<int>(80)},  {{"Fuel Capacity", 2}, std::optional<int>(800)},
        {{"Fuel Consumption", 2}, std::optional<int>(1)}, {{"Next Level Price", 2}, std::optional<int>(80)},
        {{"Goods Capacity", 3}, std::optional<int>(160)}, {{"Fuel Capacity", 3}, std::optional<int>(1600)},
        {{"Fuel Consumption", 3}, std::optional<int>(1)}, {{"Next Level Price", 3}, std::optional<int>(std::nullopt)}};
}

void Brain::SetMap(RailGraph::Graph &map)
{
    Brain::map = &map;
}

std::string Brain::FirstTrain(const RailGraph::Graph::Train &train)
{
    UpdateDist({RailGraph::Graph::Post::StorageIndex}, {});
    if (train.info.goods > 0)
    {
        if (train.info.goods == train.info.goodsCapacity)
        {
            auto path = TrainOptimalPath(train, homeIdx);
            return "move " + std::to_string(std::get<1>(path)) + " " + std::to_string(std::get<2>(path)) + " " +
                   std::to_string(train.info.idx);
        }
    }
    else
    {
        auto points = map->GetPoints();
        auto posts = map->GetPosts();
        std::tuple<int, int, int> minPath{std::numeric_limits<int>::max(), -1, -1};

        for (const auto &post : posts)
        {
            if (post.postInfo.index() == RailGraph::Graph::Post::MarketIndex &&
                std::get<RailGraph::Graph::MarketInfo>(post.postInfo).product > 0)
            {
                minPath = std::min(minPath, TrainOptimalPath(train, post.info.point_idx));
            }
        }

        if (std::get<1>(minPath) != -1)
        {
            return "move " + std::to_string(std::get<1>(minPath)) + " " + std::to_string(std::get<2>(minPath)) + " " +
                   std::to_string(train.info.idx);
        }
    }
    return "move " + std::to_string(train.info.lineIdx) + " 0 " + std::to_string(train.info.idx);
}

std::string Brain::SecondTrain(const RailGraph::Graph::Train &train)
{
    UpdateDist({RailGraph::Graph::Post::StorageIndex}, {});
    if (train.info.goods > 0)
    {
        if (train.info.goods == train.info.goodsCapacity)
        {
            auto path = TrainOptimalPath(train, homeIdx);
            return "move " + std::to_string(std::get<1>(path)) + " " + std::to_string(std::get<2>(path)) + " " +
                   std::to_string(train.info.idx);
        }
    }
    else
    {
        auto points = map->GetPoints();
        auto posts = map->GetPosts();
        std::tuple<int, int, int> minPath{std::numeric_limits<int>::max(), -1, -1};

        for (const auto &post : posts)
        {
            if (post.postInfo.index() == RailGraph::Graph::Post::MarketIndex &&
                std::get<RailGraph::Graph::MarketInfo>(post.postInfo).product > 0)
            {
                minPath = std::min(minPath, TrainOptimalPath(train, post.info.point_idx));
            }
        }

        if (std::get<1>(minPath) != -1)
        {
            return "move " + std::to_string(std::get<1>(minPath)) + " " + std::to_string(std::get<2>(minPath)) + " " +
                   std::to_string(train.info.idx);
        }
    }
    return "move " + std::to_string(train.info.lineIdx) + " 0 " + std::to_string(train.info.idx);
}

std::string Brain::RandomMove(const RailGraph::Graph::Train &train)
{
    UpdateDist({}, {});
    RailGraph::Graph::Line currentLine{0, 0, {0, 0}};
    auto edges = map->GetLines();
    for (int i = 0; i < edges.size(); ++i)
    {
        if (edges[i].idx == train.info.lineIdx)
        {
            currentLine = edges[i];
            break;
        }
    }
    if (train.info.position != 0 && train.info.position != currentLine.length && train.info.speed != 0)
    {
        return "move " + std::to_string(train.info.lineIdx) + std::to_string(train.info.speed) +
               std::to_string(train.info.idx);
    }
    else
    {
        srand(time(nullptr));
        auto points = map->GetPoints();
        size_t randomIdx = rand() % points.size();
        auto result = TrainOptimalPath(train, points[randomIdx].idx);
        return "move " + std::to_string(std::get<1>(result)) + " " + std::to_string(std::get<2>(result)) + " " +
               std::to_string(train.info.idx);
    }
}

std::vector<std::string> Brain::GetTurn(int turn)
{
    std::vector<std::string> commands;
    auto trains = map->GetTrains();
    UpdateDist({}, {});
    for (const auto &i : trains)
    {
        for (const auto &j : trains)
        {
            distBetweenTrains[i.info.idx][i.info.idx] = GetDistBetween(i, j);
        }
    }
    if (!trainsIdx.empty())
    {
        for (const auto &j : trains)
        {
            if (j.info.idx == trainsIdx[0])
            {
                commands.emplace_back(FirstTrain(j));
            }
        }
    }
    //    RailGraph::Graph::Post mainCity;
    //    for (const auto &post : map->GetPosts())
    //    {
    //        if (post.info.idx == homePostIdx)
    //        {
    //            mainCity = post;
    //        }
    //    }
    //    if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor >= 40)
    //    {
    //        if (trains[0].info.level == 1)
    //        {
    //            commands.emplace_back("upgrade {|1}");
    //            std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor -= 40;
    //        }
    //    }
    //    if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor >= 40)
    //    {
    //        if (trains[1].info.level == 1)
    //        {
    //            commands.emplace_back("upgrade {|2}");
    //            std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor -= 40;
    //        }
    //    }
    //    if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor >= 80)
    //    {
    //        if (trains[0].info.level == 2)
    //        {
    //            commands.emplace_back("upgrade {|1}");
    //            std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor -= 80;
    //        }
    //    }
    //    if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor >= 80)
    //    {
    //        if (trains[1].info.level == 2)
    //        {
    //            commands.emplace_back("upgrade {|2}");
    //            std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor -= 80;
    //        }
    //    }
    //    if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor >= 100)
    //    {
    //        if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).level == 1)
    //        {
    //            commands.emplace_back("upgrade {" + std::to_string(homePostIdx) + "|}");
    //            std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor -= 100;
    //        }
    //    }
    //    if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor >= 200)
    //    {
    //        if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).level == 2)
    //        {
    //            commands.emplace_back("upgrade {" + std::to_string(homePostIdx) + "|}");
    //            std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor -= 200;
    //        }
    //    }
    return commands;
}

void Brain::UpdateDist(const std::vector<int> &skipIndex, const std::vector<int> &skipPoints)
{
    auto edges = map->GetLines();
    auto points = map->GetPoints();
    auto posts = map->GetPosts();
    std::vector<bool> ban(200);
    for (const auto &i : posts)
    {
        for (const auto &j : skipIndex)
        {
            if (i.postInfo.index() == j)
            {
                ban[i.info.point_idx] = true;
                break;
            }
        }
    }
    for (const auto &i : skipPoints)
    {
        ban[i] = true;
    }
    for (const auto &i : points)
    {
        for (const auto &j : points)
        {
            if (i != j)
            {
                dist[i.idx][j.idx] = std::numeric_limits<int>::max() >> 2;
            }
        }
    }
    for (const auto &i : edges)
    {
        if (ban[i.points.first] || ban[i.points.second])
        {
            continue;
        }
        dist[i.points.first][i.points.second] = dist[i.points.second][i.points.first] =
            std::min<int>(dist[i.points.first][i.points.second], i.length);
        direction[i.points.first][i.points.second] = 1;
        direction[i.points.second][i.points.first] = -1;
    }
    for (const auto &k : points)
    {
        for (const auto &i : points)
        {
            for (const auto &j : points)
            {
                if (ban[i.idx] | ban[j.idx] | ban[k.idx])
                {
                    continue;
                }
                if (dist[i.idx][j.idx] > dist[i.idx][k.idx] + dist[k.idx][j.idx])
                {
                    dist[i.idx][j.idx] = dist[i.idx][k.idx] + dist[k.idx][j.idx];
                    direction[i.idx][j.idx] = direction[i.idx][k.idx];
                }
            }
        }
    }
}

std::tuple<int, int, int> Brain::TrainOptimalPath(const RailGraph::Graph::Train &train, int destination)
{
    auto edges = map->GetLines();
    for (const auto &i : edges)
    {
        if (i.idx == train.info.lineIdx)
        {
            if (train.info.position == 0 || train.info.position == i.length)
            {
                int start = (train.info.position == 0 ? i.points.first : i.points.second);
                std::tuple<int, int, int> res = {std::numeric_limits<int>::max(), -1, -1};
                for (const auto &j : edges)
                {
                    if (j.points.first == start && std::get<0>(res) > j.length + dist[j.points.second][destination])
                    {
                        res = {j.length + dist[j.points.second][destination], j.idx, direction[start][j.points.second]};
                    }
                    if (j.points.second == start && std::get<0>(res) > j.length + dist[j.points.first][destination])
                    {
                        res = {j.length + dist[j.points.first][destination], j.idx, direction[start][j.points.first]};
                    }
                }
                return res;
            }
            int u = i.points.first, v = i.points.second;
            if (dist[u][destination] + train.info.position > dist[v][destination] + (i.length - train.info.position))
            {
                return {dist[v][destination] + (i.length - train.info.position), train.info.lineIdx, 1};
            }
            else
            {
                return {dist[u][destination] + train.info.position, train.info.lineIdx, -1};
            }
        }
    }
    return {-1, -1, -1};
}

void Brain::SetHomeIdx(int homeIdx)
{
    Brain::homeIdx = homeIdx;
}

void Brain::SetHomePostIdx(int homePostIdx)
{
    Brain::homePostIdx = homePostIdx;
}

void Brain::SetTrains(std::vector<int> &trains)
{
    Brain::trainsIdx = trains;
}

const std::string &Brain::GetIdx() const
{
    return idx;
}

void Brain::SetIdx(const std::string &idx)
{
    Brain::idx = idx;
}

std::pair<int, int> Brain::PositionOnTheNextMove(const RailGraph::Graph::Train &train,
                                                 const std::tuple<int, int, int> &nextMove)
{
    auto edges = map->GetLines();
    for (const auto &i : edges)
    {
        if (i.idx == train.info.lineIdx)
        {
            if (i.length == train.info.position && i.length == 0)
            {
                for (const auto &j : edges)
                {
                    if (j.idx == std::get<0>(nextMove))
                    {
                        return {j.idx,
                                (std::get<1>(nextMove) < 0 ? j.length + std::get<1>(nextMove) : std::get<1>(nextMove))};
                    }
                }
            }
            else
            {
                return {i.idx, train.info.position + std::get<1>(nextMove)};
            }
        }
    }
    return {-1, -1};
}

bool Brain::IsCrash(std::pair<int, int> firstPosition, std::pair<int, int> secondPosition)
{
    if (firstPosition == secondPosition)
    {
        return true;
    }
    auto edges = map->GetLines();
    int firstPoint = -1, secondPoint = -1;
    for (const auto &i : edges)
    {
        if (i.idx == firstPosition.first && firstPosition.second == 0 || firstPosition.second == i.length)
        {
            firstPoint = (firstPosition.second == 0 ? i.points.first : i.points.second);
        }
        if (i.idx == secondPosition.first && secondPosition.second == 0 || secondPosition.second == i.length)
        {
            secondPoint = (secondPosition.second == 0 ? i.points.first : i.points.second);
        }
    }
    return (firstPoint == secondPoint && firstPoint != -1 && secondPoint != -1);
}

int Brain::GetDistBetween(const RailGraph::Graph::Train &train1, const RailGraph::Graph::Train &train2)
{
    if (train1.info.lineIdx == train2.info.lineIdx)
    {
        return abs(train1.info.position - train2.info.position);
    }
    auto points = map->GetPoints();
    int min = std::numeric_limits<int>::max();
    for (const auto &i : points)
    {
        min = std::min(min, dist[train1.info.idx][i.idx] + dist[train2.info.idx][i.idx]);
    }
    return min;
}
