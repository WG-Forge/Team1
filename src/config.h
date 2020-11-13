#ifndef RAIL_SIMULATOR_CONFIG_H
#define RAIL_SIMULATOR_CONFIG_H

struct Config {
  int windowX = 800;
  int windowY = 600;
  bool isFullscreen = true;
  bool hideCursor = false;
  int fps = 60;
  std::string pathToJson = "../tests/graph_tests/small_graph.json";

  Config() = default;
};

#endif // RAIL_SIMULATOR_CONFIG_H
