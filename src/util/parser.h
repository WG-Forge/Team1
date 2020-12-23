#ifndef RAIL_SIMULATOR_PARSER_H
#define RAIL_SIMULATOR_PARSER_H
#include "libs/rapidjson/document.h"
#include "src/game/graph.h"
#include <string>

namespace RailGraph
{
class GraphParser
{
  public:
    explicit GraphParser(const std::string &parseText)
    {
        document.Parse(parseText.c_str());
    }
    Graph GetGraph() const;

  private:
    rapidjson::Document document;
};

std::pair<size_t, size_t> ParseLoginFromJson(const std::string &parseText);
Graph ParseMap0FromJson(const std::string &parseText);
std::vector<Graph::Post> ParseMap1FromJson(const std::string &parseText);
std::vector<std::tuple<size_t, int, int>> ParseMap10FromJson(const std::string &parseText);
std::vector<Graph::Train> ParseTrainsFromJson(const std::string &parseText);
} // namespace RailGraph
#endif
