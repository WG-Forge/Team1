#ifndef RAIL_SIMULATOR_BRAIN_H
#define RAIL_SIMULATOR_BRAIN_H

#include <functional>
#include <iostream>
#include <map>
#include <src/game/graph.h>

class Brain
{
  private:
    void UpdateDist(const std::vector<int> &, const std::vector<int> &);
    std::tuple<int, int, int> TrainOptimalPath(const RailGraph::Graph::Train &, int);
    std::pair<int, int> PositionOnTheNextMove(const RailGraph::Graph::Train &, const std::tuple<int, int, int> &);
    bool IsCrash(std::pair<int, int> first, std::pair<int, int> second);
    int GetDistBetween(const RailGraph::Graph::Train &, const RailGraph::Graph::Train &);

    std::string FirstTrain(const RailGraph::Graph::Train &train);
    std::string SecondTrain(const RailGraph::Graph::Train &train);
    std::string ThirdTrain(const RailGraph::Graph::Train &train);
    std::string RandomMove(const RailGraph::Graph::Train &train);

  public:
    Brain();
    std::vector<std::string> GetTurn(int turn);
    void SetMap(RailGraph::Graph &map);
    void SetHomeIdx(int homeIdx);
    void SetIdx(const std::string &idx);
    void SetHomePostIdx(int homePostIdx);
    void SetTrains(std::vector<int> &);
    [[nodiscard]] const std::string &GetIdx() const;

  private:
    std::map<std::pair<std::string, int>, std::optional<int>> townLevelInformation, trainLevelInformation;
    std::array<std::array<int, 200>, 200> dist, direction, distBetweenTrains;
    std::vector<int> trainsIdx;
    RailGraph::Graph *map;
    int homeIdx, homePostIdx;
    std::string idx;
};

#endif // RAIL_SIMULATOR_BRAIN_H
