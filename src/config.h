#ifndef RAIL_SIMULATOR_CONFIG_H
#define RAIL_SIMULATOR_CONFIG_H

struct Config
{
    int windowX = 800;
    int windowY = 600;
    bool isFullscreen = true;
    bool hideCursor = false;
    bool isSoloGame = false;
    int fps = 300;
    std::string pathToJson = "../tests/graph_tests/big_graph.json";
    std::string teamName = "Sanyok2";
    std::string password = "12345";
    std::string gameName = "Gucci-flip-flop";
    int numTurns = -1;
    int numPlayers = 4;

    Config() = default;
};

#endif // RAIL_SIMULATOR_CONFIG_H
