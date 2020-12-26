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

  public:
    Brain();

    std::vector<std::string> GetTurn();
    void SetMap(RailGraph::Graph &map);
    void SetHomeIdx(int homeIdx);
    void SetIdx(const std::string &idx);
    void SetHomePostIdx(int homePostIdx);
    const std::string &GetIdx() const;
    std::tuple<int, int, int> TrainOptimalPath(const RailGraph::Graph::Train &train, int destination);

    std::string FirstTrain(const RailGraph::Graph::Train &train);
    std::string SecondTrain(const RailGraph::Graph::Train &train);

  private:
    std::map<std::pair<std::string, int>, std::optional<int>> townLevelInformation, trainLevelInformation;
    std::array<std::array<int, 200>, 200> dist, direction;
    RailGraph::Graph *map;
    int homeIdx, homePostIdx;
    std::string idx;
};

#endif // RAIL_SIMULATOR_BRAIN_H
