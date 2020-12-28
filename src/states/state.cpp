#include "state.h"

State::State(RailGraph::Graph &graph, std::vector<std::pair<sf::Text, std::string>> texts)
    : graphState(graph), staticTexts(std::move(texts))
{
    for (int i = 0; i <= 5000; i++)
    {
        Resize(-i);
    }
    Resize(400);
}

void State::AddLine(const std::vector<sf::Vertex> &line)
{
    lines.push_back(line);
}
void State::AddCircle(const sf::CircleShape &circle)
{
    circles.push_back(circle);
}
void State::AddNonStaticText(const sf::Text &text, const std::string &fontName)
{
    nonStaticTexts.emplace_back(text, fontName);
}
void State::AddStaticText(const sf::Text &text, const std::string &fontName)
{
    staticTexts.emplace_back(text, fontName);
}

const std::vector<std::vector<sf::Vertex>> &State::GetLines()
{
    State::lines.clear();
    auto points = State::graphState.GetPoints();
    auto edges = State::graphState.GetLines();
    for (const auto &edge : edges)
    {
        size_t u = graphState.GetNum(edge.points.first), v = graphState.GetNum(edge.points.second);
        State::AddLine(SfmlTool::GetLine(points[u].renderX, points[u].renderY, points[v].renderX, points[v].renderY));
    }
    return State::lines;
}
std::vector<std::pair<sf::CircleShape, std::string>> State::GetCircles()
{
    State::circles.clear();
    std::vector<std::pair<sf::CircleShape, std::string>> res;
    auto points = State::graphState.GetPoints();
    auto posts = State::graphState.GetPosts();
    for (size_t i = 0; i < points.size(); ++i)
    {
        auto circle = SfmlTool::GetCircle(points[i].renderX, points[i].renderY, circleRadius);
        State::AddCircle(circle);
        std::string type = "default";
        for (auto &post : posts)
        {
            if (post.info.point_idx == points[i].idx)
            {
                switch (post.postInfo.index())
                {
                case 0: {
                    type = "market";
                    break;
                }
                case 1: {
                    type = "town";
                    break;
                }
                case 2: {
                    type = "storage";
                    break;
                }
                default: {
                    break;
                }
                }
                break;
            }
        }
        res.emplace_back(circle, type);
    }
    return res;
}
std::vector<std::pair<sf::Text, std::string>> State::GetNonStaticTexts()
{
    nonStaticTexts.clear();
    if (showPointInformation)
    {
        for (size_t i = 0; i < State::graphState.GetPoints().size(); ++i)
        {
            nonStaticTexts.emplace_back(SfmlTool::GetText(circles[i].getPosition().x + circles[i].getRadius() * 2,
                                                          circles[i].getPosition().y + circles[i].getRadius() * 2,
                                                          std::to_string(State::graphState.GetPoints()[i].idx), 10,
                                                          sf::Color(82, 73, 73)),
                                        "8-bit-pusab");
        }
    }
    return nonStaticTexts;
}
std::vector<std::pair<sf::Text, std::string>> State::GetStaticTexts()
{
    return staticTexts;
}

std::vector<std::pair<sf::CircleShape, std::string>> State::GetTrains()
{
    std::vector<std::pair<sf::CircleShape, std::string>> res;
    auto trains = graphState.GetTrains();
    auto edges = graphState.GetLines();
    auto points = graphState.GetPoints();
    for (const auto &train : trains)
    {
        for (const auto &edge : edges)
        {
            if (edge.idx == train.info.lineIdx)
            {
                size_t u = graphState.GetNum(edge.points.first), v = graphState.GetNum(edge.points.second);
                float X = points[v].renderX - points[u].renderX, Y = points[v].renderY - points[u].renderY;
                res.emplace_back(SfmlTool::GetCircle(points[u].renderX + X / edge.length * train.info.position,
                                                     points[u].renderY + Y / edge.length * train.info.position,
                                                     circleRadius * 0.55),
                                 "train");
                break;
            }
        }
    }
    return res;
}

void State::Resize(float delta)
{
    float len = 0;
    for (auto &point : State::graphState.GetPoints())
    {
        len += RailGraph::Graph::GetRenderDist(point, RailGraph::Graph::Point());
    }
    if (!State::graphState.GetPoints().empty())
    {
        len /= State::graphState.GetPoints().size();
    }
    for (auto &point : State::graphState.GetPoints())
    {
        if (len < 100 && delta < 0)
        {
            break;
        }
        point.renderX += point.renderX / len * delta;
        point.renderY += point.renderY / len * delta;
    }
    float min = 500.f;
    for (const auto &point1 : State::graphState.GetPoints())
    {
        for (const auto &point2 : State::graphState.GetPoints())
        {
            if (point1 != point2)
            {
                min = std::min(RailGraph::Graph::GetRenderDist(point1, point2), min);
            }
        }
    }
    State::circleRadius = std::max(1.f, min / 3);
}

void State::ChangePointLocation(int index, float X, float Y)
{
    State::graphState.GetPoints()[index].renderX = X;
    State::graphState.GetPoints()[index].renderY = Y;
    State::Resize(0);
}

RailGraph::Graph::Point State::GetCenter()
{
    if (graphState.GetPoints().empty())
    {
        return RailGraph::Graph::Point();
    }
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    for (const auto &point : graphState.GetPoints())
    {
        minX = std::min<float>(minX, point.renderX);
        maxX = std::max<float>(maxX, point.renderX);
        minY = std::min<float>(minY, point.renderY);
        maxY = std::max<float>(maxY, point.renderY);
    }
    return RailGraph::Graph::Point(0, 0, (minX + maxX) / 2.f, (minY + maxY) / 2.f);
}

void State::ResetPointInformation()
{
    showPointInformation ^= true;
}

void State::UpdateTrains(std::vector<RailGraph::Graph::Train> trains)
{
    graphState.SetTrains(trains);
}

void State::UpdateRatings(std::vector<RailGraph::Graph::Rating> ratings, int width)
{
    staticTexts.clear();
    int y = 20;
    for (const auto &i : ratings)
    {
        AddStaticText(
            SfmlTool::GetText(width - 300, y, i.name + ": " + std::to_string(i.rating), 15, sf::Color(82, 73, 73)),
            "8-bit-pusab");
        y += 25;
    }
}

int State::GetCircleIndex(sf::CircleShape circleShape)
{
    auto graphPoints = graphState.GetPoints();
    for (int i = 0; i < graphPoints.size(); ++i)
    {
        if (circles[i].getPosition() == circleShape.getPosition())
        {
            return graphPoints[i].idx;
        }
    }
    return -1;
}
