#ifndef RAIL_SIMULATOR_BRAIN_H
#define RAIL_SIMULATOR_BRAIN_H

#include <iostream>
#include <map>
#include <src/game/graph.h>

class Brain
{
  private:
    void UpdateDist();

  public:
    Brain();

    void Update();
    void SetMap(RailGraph::Graph &map);
    std::tuple<int, int, int> TrainOptimalPath(RailGraph::Graph::Train &train, int destination);

  private:
    std::map<std::pair<std::string, int>, std::optional<int>> townLevelInformation, trainLevelInformation;
    std::unordered_map<int, std::unordered_map<int, int>> dist, direction;
    RailGraph::Graph map;
};

#endif // RAIL_SIMULATOR_BRAIN_H
