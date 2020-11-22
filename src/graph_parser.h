#ifndef RAIL_SIMULATOR_GRAPH_PARSER_H
#define RAIL_SIMULATOR_GRAPH_PARSER_H
#include "graph.h"
#include "libs/rapidjson/document.h"
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
} // namespace RailGraph
#endif
