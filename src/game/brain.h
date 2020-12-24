#ifndef RAIL_SIMULATOR_BRAIN_H
#define RAIL_SIMULATOR_BRAIN_H

#include <iostream>
#include <map>
#include <src/game/graph.h>

class Brain
{
  private:
    void UpdateDist(const std::vector<int> &);

  public:
    Brain();

    std::vector<std::string> GetTurn();
    void SetMap(RailGraph::Graph &map);
    void SetHomeIdx(int homeIdx);
    void SetIdx(const std::string &idx);
    void SetHomePostIdx(int homePostIdx);
    const std::string &GetIdx() const;
    std::tuple<int, int, int> TrainOptimalPath(const RailGraph::Graph::Train &train, int destination);

  private:
    std::map<std::pair<std::string, int>, std::optional<int>> townLevelInformation, trainLevelInformation;
    std::unordered_map<int, std::unordered_map<int, int>> dist, direction;
    RailGraph::Graph *map;
    int homeIdx, homePostIdx;
    std::string idx;
};

#endif // RAIL_SIMULATOR_BRAIN_H
