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
}

bool Brain::DangerFromEnemyTrains(RailGraph::Graph::Train &train, const std::tuple<int, int, int> &nextMove)
{
    auto nextPosition = PositionOnTheNextMove(train, nextMove);
    std::pair<int, int> prevPosition = {train.info.lineIdx, train.info.position};
    train.info.lineIdx = nextPosition.first, train.info.position = nextPosition.second;
    RecalcTrainDist(train);
    for (const auto &anotherTrain : map->GetTrains())
    {
        bool friendlyTrain = false;
        for (const auto &friendlyIdx : trainsIdx)
        {
            friendlyTrain |= (anotherTrain.info.idx == friendlyIdx);
        }
        if (!friendlyTrain && distBetweenTrains[train.info.idx][anotherTrain.info.idx] <= 1)
        {
            train.info.lineIdx = prevPosition.first, train.info.position = prevPosition.second;
            return true;
        }
    }
    train.info.lineIdx = prevPosition.first, train.info.position = prevPosition.second;
    return false;
}

std::tuple<int, int, int> Brain::ProductTrain(RailGraph::Graph::Train &train)
{
    int pointIdx = GetPointPosition({train.info.lineIdx, train.info.position});
    if (pointIdx == homeIdx)
    {
        banForTrain[train.info.idx].clear();
    }
    if (pointIdx != homeIdx && pointIdx != -1)
    {
        banForTrain[train.info.idx].emplace_back(pointIdx);
    }
    UpdateDist({RailGraph::Graph::Post::StorageIndex}, banForTrain[train.info.idx]);
    std::tuple<int, int, int> nextMove = {train.info.lineIdx, 0, train.info.idx};
    if (train.info.goods > 0)
    {
        if (train.info.goods == train.info.goodsCapacity)
        {
            auto path = TrainOptimalPath(train, homeIdx);
            nextMove = {std::get<1>(path), std::get<2>(path), train.info.idx};
        }
    }
    else
    {
        auto points = map->GetPoints();
        auto posts = map->GetPosts();
        std::tuple<int, int, int> minPath{std::numeric_limits<int>::max(), -1, -1};

        for (const auto &post : posts)
        {
            if (post.postInfo.index() == RailGraph::Graph::Post::MarketIndex)
            {
                minPath = std::min(minPath, TrainOptimalPath(train, post.info.point_idx));
            }
        }

        if (std::get<1>(minPath) != -1)
        {
            nextMove = {std::get<1>(minPath), std::get<2>(minPath), train.info.idx};
        }
    }
    if (!DangerFromEnemyTrains(train, nextMove) || trainWaiting[train.info.idx] > 5)
    {
        trainWaiting[train.info.idx] = 0;
        return nextMove;
    }
    else
    {
        trainWaiting[train.info.idx]++;
        return {train.info.lineIdx, 0, train.info.idx};
    }
}

std::tuple<int, int, int> Brain::ArmorTrain(RailGraph::Graph::Train &train)
{
    int pointIdx = GetPointPosition({train.info.lineIdx, train.info.position});
    if (pointIdx == homeIdx)
    {
        banForTrain[train.info.idx].clear();
    }
    if (pointIdx != homeIdx && pointIdx != -1)
    {
        banForTrain[train.info.idx].emplace_back(pointIdx);
    }
    UpdateDist({RailGraph::Graph::Post::MarketIndex}, banForTrain[train.info.idx]);
    std::tuple<int, int, int> nextMove = {train.info.lineIdx, 0, train.info.idx};
    if (train.info.goods > 0)
    {
        if (train.info.goods == train.info.goodsCapacity ||
            train.info.goods + GetCurrentArmor() >= GetUpgradesSum() + 30)
        {
            auto path = TrainOptimalPath(train, homeIdx);
            nextMove = {std::get<1>(path), std::get<2>(path), train.info.idx};
        }
    }
    else
    {
        auto points = map->GetPoints();
        auto posts = map->GetPosts();
        std::tuple<int, int, int> minPath{std::numeric_limits<int>::max(), -1, -1};

        for (const auto &post : posts)
        {
            if (post.postInfo.index() == RailGraph::Graph::Post::StorageIndex)
            {
                minPath = std::min(minPath, TrainOptimalPath(train, post.info.point_idx));
            }
        }

        if (std::get<1>(minPath) != -1)
        {
            nextMove = {std::get<1>(minPath), std::get<2>(minPath), train.info.idx};
        }
    }
    if (!DangerFromEnemyTrains(train, nextMove) || trainWaiting[train.info.idx] > 5)
    {
        trainWaiting[train.info.idx] = 0;
        return nextMove;
    }
    else
    {
        trainWaiting[train.info.idx]++;
        return {train.info.lineIdx, 0, train.info.idx};
    }
}

std::vector<std::string> Brain::GetTurn(int turn)
{
    static bool zeroGetArmor = false, zeroDeliveredArmor = false;
    static bool firstGetArmor = false, firstDeliveredArmor = false;
    static bool secondGetArmor = false, secondDeliveredArmor = false;
    static bool thirdGetArmor = false, thirdDeliveredArmor = false;
    static bool allUpgraded = false;
    CalculateTrainsDist();
    std::vector<std::string> commands;
    int currentArmor = GetCurrentArmor();

    std::vector<RailGraph::Graph::Train> trains;
    for (const auto &trainIdx : trainsIdx)
    {
        for (const auto &train : map->GetTrains())
        {
            if (train.info.idx == trainIdx)
            {
                trains.emplace_back(train);
            }
        }
    }

    if (trains[3].info.level == 3)
    {
        currentArmor -= 15;
    }
    if (trains[0].info.goods > 0)
    {
        zeroGetArmor = true;
    }
    else if (firstGetArmor && trains[0].info.goods == 0)
    {
        zeroDeliveredArmor = true;
    }
    if (trains[1].info.goods > 0)
    {
        firstGetArmor = true;
    }
    else if (firstGetArmor && trains[1].info.goods == 0)
    {
        firstDeliveredArmor = true;
    }
    if (trains[2].info.goods > 0)
    {
        secondGetArmor = true;
    }
    else if (secondGetArmor && trains[2].info.goods == 0)
    {
        secondDeliveredArmor = true;
    }

    std::vector<std::tuple<int, int, int>> moveTurns;
    bool upgradeHome = false;
    std::vector<int> upgradeTrains;

    int sumTrainsUpgrade = 0;
    for (const auto &train : trains)
    {
        sumTrainsUpgrade += train.info.level;
    }
    if (GetUpgradesSum() <= currentArmor)
    {
        std::cerr << GetUpgradesSum() << ' ' << currentArmor << '\n';
        allUpgraded = true;
    }

    if (sumTrainsUpgrade >= 8)
    {
        if (UpgradeHome(currentArmor))
        {
            upgradeHome = true;
        }
    }
    else if (sumTrainsUpgrade >= 10)
    {
        if (UpgradeHome(currentArmor))
        {
            upgradeHome = true;
        }
    }

    // --------------------------------------------- Train 4 -----------------------------------------------------------

    if (trains[3].info.level != 3)
    {
        if (UpgradeTrain(trains[3], currentArmor))
        {
            upgradeTrains.emplace_back(trains[3].info.idx);
        }
        UpgradeTrain(trains[3], currentArmor);
    }
    else if (!allUpgraded)
    {
        moveTurns.emplace_back(ArmorTrain(trains[3]));
    }
    else if (std::get<0>(TrainOptimalPath(trains[3], homeIdx)) > 1 || trains[3].info.goodsType == 3 ||
             trains[3].info.goods <= GetFreeFoodSpace())
    {
        moveTurns.emplace_back(ProductTrain(trains[3]));
    }
    else
    {
        moveTurns.emplace_back(WaitMove(trains[3]));
    }

    // --------------------------------------------- Train 1 -----------------------------------------------------------

    if (std::get<0>(TrainOptimalPath(trains[0], homeIdx)) == 0 && UpgradeTrain(trains[0], currentArmor))
    {
        upgradeTrains.emplace_back(trains[0].info.idx);
        UpgradeTrain(trains[0], currentArmor);
    }
    else if (!zeroGetArmor || !zeroDeliveredArmor)
    {
        moveTurns.emplace_back(ArmorTrain(trains[0]));
    }
    else if (std::get<0>(TrainOptimalPath(trains[0], homeIdx)) > 1 || trains[0].info.goodsType == 3 ||
             trains[0].info.goods <= GetFreeFoodSpace())
    {
        moveTurns.emplace_back(ProductTrain(trains[0]));
    }
    else
    {
        moveTurns.emplace_back(WaitMove(trains[0]));
    }

    // --------------------------------------------- Train 2 -----------------------------------------------------------

    if (std::get<0>(TrainOptimalPath(trains[1], homeIdx)) == 0 && UpgradeTrain(trains[1], currentArmor))
    {
        upgradeTrains.emplace_back(trains[1].info.idx);
        UpgradeTrain(trains[1], currentArmor);
    }
    else if (!firstGetArmor || !firstDeliveredArmor)
    {
        moveTurns.emplace_back(ArmorTrain(trains[1]));
    }
    else if (std::get<0>(TrainOptimalPath(trains[1], homeIdx)) > 1 || trains[1].info.goodsType == 3 ||
             trains[1].info.goods <= GetFreeFoodSpace())
    {
        moveTurns.emplace_back(ProductTrain(trains[1]));
    }
    else
    {
        moveTurns.emplace_back(WaitMove(trains[1]));
    }

    // --------------------------------------------- Train 3 -----------------------------------------------------------

    if (std::get<0>(TrainOptimalPath(trains[2], homeIdx)) == 0 && UpgradeTrain(trains[2], currentArmor))
    {
        upgradeTrains.emplace_back(trains[2].info.idx);
        UpgradeTrain(trains[2], currentArmor);
    }
    else if (!secondGetArmor || !secondDeliveredArmor)
    {
        moveTurns.emplace_back(ArmorTrain(trains[2]));
    }
    else if (std::get<0>(TrainOptimalPath(trains[2], homeIdx)) > 1 || trains[2].info.goodsType == 3 ||
             trains[2].info.goods <= GetFreeFoodSpace())
    {
        moveTurns.emplace_back(ProductTrain(trains[2]));
    }
    else
    {
        moveTurns.emplace_back(WaitMove(trains[2]));
    }

    // --------------------------------------------- Upgrade -----------------------------------------------------------

    commands.emplace_back(Upgrade(upgradeHome, upgradeTrains));
    if (commands.back() == "upgrade {|}")
    {
        commands.pop_back();
    }

    // ----------------------------------------------- End -------------------------------------------------------------

    std::sort(std::begin(moveTurns), std::end(moveTurns),
              [](const std::tuple<int, int, int> &lhs, const std::tuple<int, int, int> &rhs) {
                  if (std::get<1>(lhs) == 0)
                  {
                      return true;
                  }
                  else if (std::get<1>(rhs) == 0)
                  {
                      return false;
                  }
                  else
                  {
                      return lhs < rhs;
                  }
              });
    std::vector<std::pair<int, int>> nextPosition(moveTurns.size());
    std::vector<std::pair<int, int>> waitPosition(moveTurns.size());
    std::vector<int> trainIdx(moveTurns.size());
    for (int i = 0; i < nextPosition.size(); ++i)
    {
        for (const auto &train : trains)
        {
            if (train.info.idx == std::get<2>(moveTurns[i]))
            {
                trainIdx[i] = train.info.idx;
                nextPosition[i] = PositionOnTheNextMove(train, moveTurns[i]);
                waitPosition[i] = {train.info.lineIdx, train.info.position};
            }
        }
    }

    for (int i = 0; i < moveTurns.size(); ++i)
    {
        bool valid = true;
        for (int j = 0; j < i; ++j)
        {
            if (IsCrash(nextPosition[j], nextPosition[i]))
            {
                valid = false;
                break;
            }
        }
        if (valid)
        {
            if (prevMove[std::get<2>(moveTurns[i])] != moveTurns[i])
            {
                commands.emplace_back("move " + std::to_string(std::get<0>(moveTurns[i])) + " " +
                                      std::to_string(std::get<1>(moveTurns[i])) + " " +
                                      std::to_string(std::get<2>(moveTurns[i])));
                prevMove[std::get<2>(moveTurns[i])] = moveTurns[i];
            }
        }
        else
        {
            nextPosition[i] = waitPosition[i];
            std::get<1>(moveTurns[i]) = 0;
            if (prevMove[std::get<2>(moveTurns[i])] != moveTurns[i])
            {
                commands.emplace_back("move " + std::to_string(std::get<0>(moveTurns[i])) + " 0 " +
                                      std::to_string(std::get<2>(moveTurns[i])));
                prevMove[std::get<2>(moveTurns[i])] = moveTurns[i];
            }
        }
    }

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
                if (start == destination)
                {
                    return {0, train.info.lineIdx, 0};
                }
                std::tuple<int, int, int> res = {std::numeric_limits<int>::max() >> 1, -1, -1};
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
            if (i.length == train.info.position || train.info.position == 0)
            {
                for (const auto &j : edges)
                {
                    if (j.idx == std::get<0>(nextMove))
                    {
                        if (std::get<1>(nextMove) == 0)
                        {
                            return {train.info.lineIdx, train.info.position};
                        }
                        else
                        {
                            return {j.idx, (std::get<1>(nextMove) < 0 ? j.length + std::get<1>(nextMove)
                                                                      : std::get<1>(nextMove))};
                        }
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
    auto pos1 = GetPointPosition(firstPosition);
    auto pos2 = GetPointPosition(secondPosition);
    if (pos1 != -1 && pos2 != -1 && pos1 == pos2)
    {
        for (const auto &i : map->GetPosts())
        {
            if (i.info.point_idx == pos1 && i.info.point_idx == pos2 &&
                i.postInfo.index() == RailGraph::Graph::Post::CityIndex)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }

    return firstPosition == secondPosition;
}

int Brain::GetDistBetween(const RailGraph::Graph::Train &train1, const RailGraph::Graph::Train &train2)
{
    if (train1.info.lineIdx == train2.info.lineIdx)
    {
        return abs(train1.info.position - train2.info.position);
    }
    auto points = map->GetPoints();
    int min = std::numeric_limits<int>::max();
    for (const auto &edge : map->GetLines())
    {
        if (edge.idx == train1.info.lineIdx)
        {
            return std::min(std::get<0>(TrainOptimalPath(train1, edge.points.first)) +
                                std::get<0>(TrainOptimalPath(train2, edge.points.first)),
                            std::get<0>(TrainOptimalPath(train1, edge.points.second)) +
                                std::get<0>(TrainOptimalPath(train2, edge.points.second)));
        }
    }

    return min;
}

int Brain::GetPointPosition(const std::pair<int, int> &position)
{
    auto edges = map->GetLines();
    for (const auto &i : edges)
    {
        if (i.idx == position.first)
        {
            if (position.second == i.length || position.second == 0)
            {
                return (position.second == i.length ? i.points.second : i.points.first);
            }
            else
            {
                return -1;
            }
        }
    }
    return -1;
}

void Brain::CalculateTrainsDist()
{
    auto trains = map->GetTrains();
    UpdateDist({}, {});
    for (const auto &i : trains)
    {
        for (const auto &j : trains)
        {
            distBetweenTrains[i.info.idx][j.info.idx] = GetDistBetween(i, j);
        }
    }
}

void Brain::RecalcTrainDist(const RailGraph::Graph::Train &train)
{
    auto trains = map->GetTrains();
    UpdateDist({}, {});
    for (const auto &i : trains)
    {
        if (i.info.idx != train.info.idx)
        {
            distBetweenTrains[train.info.idx][i.info.idx] = GetDistBetween(train, i);
        }
    }
}

bool Brain::UpgradeTrain(RailGraph::Graph::Train &train, int &armor)
{
    if (train.info.level == 1)
    {
        if (armor >= 30)
        {
            train.info.level++;
            armor -= 30;
            return true;
        }
    }
    else if (train.info.level == 2)
    {
        if (armor >= 60)
        {
            train.info.level++;
            armor -= 60;
            return true;
        }
    }
    return false;
}

bool Brain::UpgradeHome(int &armor)
{
    RailGraph::Graph::Post mainCity;
    for (auto &post : map->GetPosts())
    {
        if (post.info.idx == homePostIdx)
        {
            mainCity = post;
        }
    }
    if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).level == 1)
    {
        if (armor >= 75)
        {
            armor -= 75;
            return true;
        }
    }
    else if (std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).level == 2)
    {
        if (armor >= 150)
        {
            armor -= 150;
            return true;
        }
    }
    return false;
}

std::string Brain::Upgrade(bool homeUpgrade, const std::vector<int> &upgradeTrains)
{
    std::string command = "upgrade {";
    if (homeUpgrade)
    {
        command += std::to_string(homePostIdx);
    }
    command += "|";
    for (const auto &trainIdx : upgradeTrains)
    {
        command += std::to_string(trainIdx) + ",";
    }
    command += "}";
    return command;
}

int Brain::GetCurrentArmor()
{
    RailGraph::Graph::Post mainCity;
    for (auto &post : map->GetPosts())
    {
        if (post.info.idx == homePostIdx)
        {
            mainCity = post;
        }
    }
    return std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).armor;
}

std::tuple<int, int, int> Brain::RandomMove(RailGraph::Graph::Train &train)
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
        return {train.info.lineIdx, train.info.speed, train.info.idx};
    }
    else
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        auto points = map->GetPoints();
        std::uniform_int_distribution<> distrib(0, points.size());
        size_t randomIdx = distrib(gen);
        auto result = TrainOptimalPath(train, 81);
        return {std::get<1>(result), std::get<2>(result), train.info.idx};
    }
}

int Brain::GetHomeLevel()
{
    RailGraph::Graph::Post mainCity;
    for (auto &post : map->GetPosts())
    {
        if (post.info.idx == homePostIdx)
        {
            mainCity = post;
        }
    }
    return std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).level;
}

int Brain::GetUpgradesSum()
{
    int upgradesSum = 0;
    for (const auto &trainIdx : trainsIdx)
    {
        for (const auto &train : map->GetTrains())
        {
            if (train.info.idx == trainIdx)
            {
                if (train.info.level == 1)
                {
                    upgradesSum += 90;
                }
                else if (train.info.level == 2)
                {
                    upgradesSum += 60;
                }
            }
        }
    }

    if (GetHomeLevel() == 1)
    {
        upgradesSum += 225;
    }
    else if (GetHomeLevel() == 2)
    {
        upgradesSum += 150;
    }

    return upgradesSum;
}

std::tuple<int, int, int> Brain::WaitMove(RailGraph::Graph::Train &train)
{
    return {train.info.lineIdx, 0, train.info.idx};
}

int Brain::GetFreeFoodSpace()
{
    std::vector<RailGraph::Graph::Train> trains;
    for (const auto &trainIdx : trainsIdx)
    {
        for (const auto &train : map->GetTrains())
        {
            if (train.info.idx == trainIdx)
            {
                trains.emplace_back(train);
            }
        }
    }

    int trainsSum = 0;
    for (const auto &train : trains)
    {
        if (std::get<0>(TrainOptimalPath(train, homeIdx)) == 0)
        {
            trainsSum += train.info.goods;
        }
    }

    RailGraph::Graph::Post mainCity;
    for (auto &post : map->GetPosts())
    {
        if (post.info.idx == homePostIdx)
        {
            mainCity = post;
        }
    }

    return std::max(0, std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).productCapacity -
                           std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).product - trainsSum);
}

int Brain::GetPopulation()
{
    RailGraph::Graph::Post mainCity;
    for (auto &post : map->GetPosts())
    {
        if (post.info.idx == homePostIdx)
        {
            mainCity = post;
        }
    }
    return std::get<RailGraph::Graph::CityInfo>(mainCity.postInfo).population;
}