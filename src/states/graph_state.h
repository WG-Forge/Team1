#ifndef RAIL_SIMULATOR_GRAPH_STATE_H
#define RAIL_SIMULATOR_GRAPH_STATE_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/gursoy_atun_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/progress.hpp>
#include <boost/random/linear_congruential.hpp>

#include <iostream>
#include <vector>

#include "src/graph/graph.h"

class GraphState
{
  public:
    struct Point
    {
        Point &operator=(const Point &rhs);
        bool operator==(const Point &rhs) const;
        bool operator!=(const Point &rhs) const;
        float x = 0, y = 0;
        size_t idx{};
        Point(float x_, float y_, size_t idx_) : x(x_), y(y_), idx(idx_){};
        Point() = default;
    };
    static float GetDist(const Point &point1, const Point &point2);

  public:
    GraphState() = default;
    GraphState(std::vector<Point> points, std::vector<std::vector<int>> adjencyList);

    std::vector<Point> &GetPoints();
    const std::vector<std::vector<int>> &GetAdjencyList() const;
    void SetPoints(const std::vector<Point> &points);
    void SetAdjencyList(const std::vector<std::vector<int>> &adjencyList);

  private:
    std::vector<Point> points;
    std::vector<std::vector<int>> adjencyList;
};

GraphState CreateReingoldGraphStateFromGraph(const RailGraph::Graph &railGraph);

#endif // RAIL_SIMULATOR_GRAPH_STATE_H