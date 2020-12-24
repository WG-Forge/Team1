#include "brain.h"

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
    UpdateDist({RailGraph::Graph::Post::StorageIndex});
}

std::vector<std::string> Brain::GetTurn()
{

    std::vector<std::string> commands;
    auto trains = map->GetTrains();
    if (trains.front().info.goods > 0)
    {
        auto path = TrainOptimalPath(trains.front(), homeIdx);
        commands.emplace_back("move " + std::to_string(std::get<1>(path)) + " " + std::to_string(std::get<2>(path)) +
                              " " + std::to_string(trains.front().info.idx));
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
                minPath = std::min(minPath, TrainOptimalPath(trains.front(), post.info.point_idx));
            }
        }

        if (std::get<1>(minPath) != -1)
        {
            commands.emplace_back("move " + std::to_string(std::get<1>(minPath)) + " " +
                                  std::to_string(std::get<2>(minPath)) + " " + std::to_string(trains.front().info.idx));
        }
    }
    commands.emplace_back("turn");
    commands.emplace_back("map 1");
    return commands;
}

void Brain::UpdateDist(const std::vector<int> &skip)
{
    auto edges = map->GetLines();
    auto points = map->GetPoints();
    auto posts = map->GetPosts();
    std::unordered_map<int, bool> ban;
    for (const auto &i : posts)
    {
        for (const auto &j : skip)
        {
            if (i.postInfo.index() == j)
            {
                ban[i.info.point_idx] = true;
                break;
            }
        }
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
                if (ban[i.idx] || ban[j.idx] || ban[k.idx])
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

const std::string &Brain::GetIdx() const
{
    return idx;
}

void Brain::SetIdx(const std::string &idx)
{
    Brain::idx = idx;
}
