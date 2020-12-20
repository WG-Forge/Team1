#ifndef RAIL_SIMULATOR_BRAIN_H
#define RAIL_SIMULATOR_BRAIN_H

#include <map>
#include <src/game/graph.h>

class Brain
{
  public:
    explicit Brain(RailGraph::Graph &map);

  private:
    std::map<std::pair<std::string, int>, std::optional<int>> townLevelInformation, trainLevelInformation;
    RailGraph::Graph map;
};

#endif // RAIL_SIMULATOR_BRAIN_H
