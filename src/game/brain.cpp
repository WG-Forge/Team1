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
    UpdateDist(
        std::vector<
            std::variant<RailGraph::Graph::MarketInfo, RailGraph::Graph::CityInfo, RailGraph::Graph::StorageInfo>>{});
}

std::vector<std::string> Brain::GetTurn()
{
    auto trains = map->GetTrains();
    auto path = TrainOptimalPath(trains.front(), 386);
    std::cout << trains.front().info.position << "\n";
    return {"move " + std::to_string(std::get<1>(path)) + " " + std::to_string(std::get<2>(path)) + " " +
            std::to_string(trains.front().info.idx), "turn", "map 1"};
}

void Brain::UpdateDist(const std::vector<std::variant<RailGraph::Graph::MarketInfo, RailGraph::Graph::CityInfo,
                                                      RailGraph::Graph::StorageInfo>> &skip)
{
    auto edges = map->GetLines();
    auto points = map->GetPoints();
    auto posts = map->GetPosts();
    std::unordered_map<int, bool> ban;
    for (const auto &i : posts)
    {
        for (const auto &j : skip)
        {
            if (i.postInfo.index() == j.index())
            {
                ban[i.info.idx] = true;
                break;
            }
        }
    }
    for (const auto &i : points)
    {
        for (const auto &j : points)
        {
            dist[i.idx][j.idx] = std::numeric_limits<int>::max() >> 1;
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

std::tuple<int, int, int> Brain::TrainOptimalPath(RailGraph::Graph::Train &train, int destination)
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
                    if (j.points.first == start && std::get<0>(res) > dist[start][destination])
                    {
                        res = {dist[start][destination], j.idx, direction[start][destination]};
                    }
                    if (j.points.second == start && std::get<0>(res) > dist[start][destination])
                    {
                        res = {dist[start][destination], j.idx, direction[start][destination]};
                    }
                }
                std::cout << std::get<0>(res) << ' ' << std::get<1>(res) << ' ' << std::get<2>(res) << '\n';
                return res;
            }
            int u = i.points.first, v = i.points.second;
            if (dist[u][destination] + train.info.position > dist[v][destination] + (i.length - train.info.position))
            {
                return {dist[v][destination] + (i.length - train.info.position), train.info.lineIdx,
                        direction[v][destination]};
            }
            return {dist[u][destination] + train.info.position, train.info.lineIdx, direction[u][destination]};
        }
    }
    return {-1, -1, -1};
}