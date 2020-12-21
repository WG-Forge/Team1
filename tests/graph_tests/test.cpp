#include <fstream>
#include <gtest/gtest.h>
#include <src/util/parser.h>

using namespace RailGraph;

TEST(small_test, just_test)
{
    std::ifstream fin("../tests/graph_tests/small_graph.json");
    std::stringstream ss;
    ss << fin.rdbuf();

    GraphParser parser(ss.str());
    auto graph = parser.GetGraph();

    std::string name = "map02";
    size_t idx = 1;
    std::vector<Graph::Point> points = {Graph::Point(1, 1), Graph::Point(2),    Graph::Point(3, 2), Graph::Point(4, 3),
                                        Graph::Point(5, 4), Graph::Point(6, 6), Graph::Point(7, 5), Graph::Point(8),
                                        Graph::Point(9),    Graph::Point(10),   Graph::Point(11),   Graph::Point(12)};
    std::vector<Graph::Line> lines = {
        Graph::Line(1, 1, {1, 7}),    Graph::Line(2, 1, {8, 2}),    Graph::Line(3, 1, {9, 3}),
        Graph::Line(4, 1, {10, 4}),   Graph::Line(5, 1, {11, 5}),   Graph::Line(6, 2, {12, 6}),
        Graph::Line(7, 1, {7, 8}),    Graph::Line(8, 2, {8, 9}),    Graph::Line(9, 2, {9, 10}),
        Graph::Line(10, 1, {10, 11}), Graph::Line(11, 3, {11, 12}), Graph::Line(12, 1, {12, 7}),
        Graph::Line(13, 2, {1, 2}),   Graph::Line(14, 2, {2, 3}),   Graph::Line(15, 1, {3, 4}),
        Graph::Line(16, 3, {4, 5}),   Graph::Line(17, 1, {5, 6}),   Graph::Line(18, 3, {6, 1})};

    ASSERT_EQ(graph.GetName(), name);
    ASSERT_EQ(graph.GetPoints(), points);
    ASSERT_EQ(graph.GetLines(), lines);
    ASSERT_EQ(graph.GetIdx(), idx);
}
