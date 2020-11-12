#include <fstream>
#include <gtest/gtest.h>
#include <src/graph_parser.h>

TEST(small_test, just_test) {
  std::ifstream fin("../tests/graph_tests/small_graph.json");
  std::stringstream ss;
  ss << fin.rdbuf();

  GraphParser parser(ss.str());
  auto graph = parser.GetGraph();

  std::string name = "map02";
  size_t idx = 1;
  std::vector<graph::Point> points = {
          graph::Point(1, 1), graph::Point(2), graph::Point(3, 2),
          graph::Point(4, 3), graph::Point(5, 4), graph::Point(6, 6),
          graph::Point(7, 5), graph::Point(8), graph::Point(9),
          graph::Point(10), graph::Point(11), graph::Point(12)};
  std::vector<graph::Line> lines = {
          graph::Line(1, 1, {1, 7}), graph::Line(2, 1, {8, 2}),
          graph::Line(3, 1, {9, 3}), graph::Line(4, 1, {10, 4}),
          graph::Line(5, 1, {11, 5}), graph::Line(6, 2, {12, 6}),
          graph::Line(7, 1, {7, 8}), graph::Line(8, 2, {8, 9}),
          graph::Line(9, 2, {9, 10}), graph::Line(10, 1, {10, 11}),
          graph::Line(11, 3, {11, 12}), graph::Line(12, 1, {12, 7}),
          graph::Line(13, 2, {1, 2}), graph::Line(14, 2, {2, 3}),
          graph::Line(15, 1, {3, 4}), graph::Line(16, 3, {4, 5}),
          graph::Line(17, 1, {5, 6}), graph::Line(18, 3, {6, 1})};

  ASSERT_EQ(graph.GetName(), name);
  ASSERT_EQ(graph.GetPoints(), points);
  ASSERT_EQ(graph.GetLines(), lines);
  ASSERT_EQ(graph.GetIdx(), idx);
}
