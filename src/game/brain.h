#ifndef RAIL_SIMULATOR_BRAIN_H
#define RAIL_SIMULATOR_BRAIN_H

#include <SFML/System/Clock.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <src/game/graph.h>
#include <unordered_set>

class Brain
{
  private:
    void UpdateDist(const std::vector<int> &, const std::vector<int> &);
    std::tuple<int, int, int> TrainOptimalPath(const RailGraph::Graph::Train &, int);
    std::pair<int, int> PositionOnTheNextMove(const RailGraph::Graph::Train &, const std::tuple<int, int, int> &);
    bool IsCrash(std::pair<int, int> first, std::pair<int, int> second);
    int GetDistBetween(const RailGraph::Graph::Train &, const RailGraph::Graph::Train &);
    int GetPointPosition(const std::pair<int, int> &position);
    int GetCurrentArmor();
    int GetHomeLevel();
    int GetUpgradesSum();
    int GetFreeFoodSpace();
    int GetPopulation();

  private:
    bool DangerFromEnemyTrains(RailGraph::Graph::Train &train, const std::tuple<int, int, int> &nextMove);
    std::string Upgrade(bool homeUpgrade, const std::vector<int> &upgradeTrains);
    bool UpgradeTrain(RailGraph::Graph::Train &train, int &armor);
    bool UpgradeHome(int &armor);
    void CalculateTrainsDist();
    void RecalcTrainDist(const RailGraph::Graph::Train &train);

    std::tuple<int, int, int> ProductTrain(RailGraph::Graph::Train &train);
    std::tuple<int, int, int> ArmorTrain(RailGraph::Graph::Train &train);
    std::tuple<int, int, int> RandomMove(RailGraph::Graph::Train &train);
    std::tuple<int, int, int> WaitMove(RailGraph::Graph::Train &train);

  public:
    Brain();
    std::vector<std::string> GetTurn(int turn);
    void SetMap(RailGraph::Graph &map);
    void SetHomeIdx(int homeIdx);
    void SetIdx(const std::string &idx);
    void SetHomePostIdx(int homePostIdx);
    void SetTrains(std::vector<int> &);
    const std::string &GetIdx() const;

  private:
    std::map<std::pair<std::string, int>, std::optional<int>> townLevelInformation, trainLevelInformation;
    std::array<std::array<int, 200>, 200> dist, direction, distBetweenTrains;
    std::unordered_map<int, int> trainWaiting;
    std::unordered_map<int, std::tuple<int, int, int>> prevMove;
    std::vector<int> trainsIdx;
    std::unordered_map<int, std::vector<int>> banForTrain;
    RailGraph::Graph *map;
    int homeIdx, homePostIdx;
    std::string idx;
};

#endif // RAIL_SIMULATOR_BRAIN_H
