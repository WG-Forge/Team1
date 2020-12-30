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

std::tuple<size_t, size_t, std::string> ParseLoginFromJson(const std::string &parseText);
Graph ParseMap0FromJson(const std::string &parseText);
std::vector<Graph::Post> ParseMap1FromJson(const std::string &parseText);
std::vector<std::tuple<size_t, int, int>> ParseMap10FromJson(const std::string &parseText);
std::vector<Graph::Train> ParseTrainsFromJson(const std::string &parseText);
std::vector<Graph::Rating> ParseRatingFromJson(const std::string &parseText, const std::vector<std::string> &);
int ParseStatusOfGameFromJson(const std::string &parseText, const std::string &game);
} // namespace RailGraph
#endif
