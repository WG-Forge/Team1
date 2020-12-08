#include "graph_state.h"

GraphState::GraphState(std::vector<GraphState::Point> points, std::vector<std::vector<int>> adjencyList)
    : points(std::move(points)), adjencyList(std::move(adjencyList))
{
}

bool GraphState::Point::operator==(const GraphState::Point &rhs) const
{
    return x == rhs.x && y == rhs.y && idx == rhs.idx;
}

bool GraphState::Point::operator!=(const GraphState::Point &rhs) const
{
    return !(rhs == *this);
}

GraphState::Point &GraphState::Point::operator=(const GraphState::Point &rhs)
{
    x = rhs.x, y = rhs.y;
    idx = rhs.idx;
    return *this;
}

std::vector<GraphState::Point> &GraphState::GetPoints()
{
    return points;
}

const std::vector<std::vector<int>> &GraphState::GetAdjencyList() const
{
    return adjencyList;
}

void GraphState::SetPoints(const std::vector<Point> &points)
{
    GraphState::points = points;
}

void GraphState::SetAdjencyList(const std::vector<std::vector<int>> &adjencyList)
{
    GraphState::adjencyList = adjencyList;
}

float GraphState::GetDist(const GraphState::Point &point1, const GraphState::Point &point2)
{
    return sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}

GraphState CreateGraphStateFromGraph(const RailGraph::Graph &railGraph)
{
    std::vector<GraphState::Point> vertices(railGraph.GetPoints().size());
    std::vector<std::vector<int>> adjencyList(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i] = {static_cast<float>(railGraph.GetPoints()[i].x), static_cast<float>(railGraph.GetPoints()[i].y),
                       railGraph.GetPoints()[i].idx};
    }
    for (const auto &line : railGraph.GetLines())
    {
        size_t firstNum = railGraph.GetNum(line.points.first), secondNum = railGraph.GetNum(line.points.second);
        adjencyList[firstNum].emplace_back(secondNum);
        adjencyList[secondNum].emplace_back(firstNum);
    }

    return GraphState(std::move(vertices), std::move(adjencyList));
}

GraphState CreateReingoldGraphStateFromGraph(const RailGraph::Graph &railGraph)
{
    using namespace boost;
    typedef square_topology<>::point_type Point;

    struct VertexProperties
    {
        VertexProperties() = default;
        std::size_t index;
        Point point;
    };
    struct EdgeProperty
    {
        explicit EdgeProperty(const std::size_t &w) : weight(w)
        {
        }
        double weight;
    };

    typedef adjacency_list<vecS, vecS, undirectedS, VertexProperties, EdgeProperty> Graph;
    typedef property_map<Graph, std::size_t VertexProperties::*>::type VertexIndexPropertyMap;
    typedef property_map<Graph, Point VertexProperties::*>::type PositionMap;
    typedef property_map<Graph, double EdgeProperty::*>::type WeightPropertyMap;
    typedef graph_traits<Graph>::vertex_descriptor VirtexDescriptor;

    Graph graph;
    VertexIndexPropertyMap vertexIdPropertyMap = get(&VertexProperties::index, graph);

    const auto &points = railGraph.GetPoints();
    const auto &lines = railGraph.GetLines();
    for (const auto &point : points)
    {
        VirtexDescriptor vd = add_vertex(graph);
        vertexIdPropertyMap[vd] = railGraph.GetNum(point.idx);
    }
    for (const auto &line : lines)
    {
        add_edge(vertex(railGraph.GetNum(line.points.first), graph),
                 vertex(railGraph.GetNum(line.points.second), graph), EdgeProperty(line.length), graph);
    }

    PositionMap positionMap = get(&VertexProperties::point, graph);
    minstd_rand gen;
    rectangle_topology<> topo(gen, -1200.0 / 2, -1200.0 / 2, 1200.0 / 2, 1200.0 / 2);
    random_graph_layout(graph, positionMap, topo);
    gursoy_atun_layout(graph, topo, positionMap, 1000);

    std::vector<GraphState::Point> vertices(railGraph.GetPoints().size());
    std::vector<std::vector<int>> adjencyList(vertices.size());
    graph_traits<Graph>::vertex_iterator i, end;
    for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i)
    {
        vertices[vertexIdPropertyMap[*i]] = {static_cast<float>(115 * positionMap[*i][0]),
                                             static_cast<float>(115 * positionMap[*i][1]),
                                             railGraph.GetPoints()[vertexIdPropertyMap[*i]].idx};
    }
    for (const auto &line : railGraph.GetLines())
    {
        size_t firstNum = railGraph.GetNum(line.points.first), secondNum = railGraph.GetNum(line.points.second);
        adjencyList[firstNum].emplace_back(secondNum);
        adjencyList[secondNum].emplace_back(firstNum);
    }
    return GraphState(std::move(vertices), std::move(adjencyList));
}