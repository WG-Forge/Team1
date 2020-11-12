#ifndef RAIL_SIMULATOR_GRAPH_PARSER_H
#define RAIL_SIMULATOR_GRAPH_PARSER_H
#include "graph.h"
#include "libs/rapidjson/document.h"
#include <string>

class GraphParser {
public:
  explicit GraphParser(const std::string &parseText) {
    document_.Parse(parseText.c_str());
  }
  graph GetGraph() const;

private:
  rapidjson::Document document_;
};

#endif
