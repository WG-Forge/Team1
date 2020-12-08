#ifndef RAIL_SIMULATOR_GRAPH_H
#define RAIL_SIMULATOR_GRAPH_H
#include <unordered_map>
#include <variant>
#include <vector>

namespace RailGraph
{
class Graph
{
  public:
    struct Info
    {
        size_t idx, point_idx;
        std::string name;
    };
    struct MarketInfo
    {
        int product, productCapacity;
        int replenishment;
    };
    struct CityInfo
    {
        int armor, armorCapacity;
        int level;
        std::optional<int> nextLevelPrice;
        int population, populationCapacity;
        int product, productCapacity;
        int trainCooldown;
        std::optional<std::string> playerIdx;
    };
    struct StorageInfo
    {
        int armor, armorCapacity;
        int replenishment;
    };
    struct Post
    {
        Post(const Info &info, const std::variant<MarketInfo, CityInfo, StorageInfo> &postInfo);
        Info info;
        std::variant<MarketInfo, CityInfo, StorageInfo> postInfo;
    };
    struct Point
    {
        explicit Point(size_t idx, std::optional<size_t> postIdx = std::nullopt);
        bool operator==(const Point &rhs) const;
        bool operator!=(const Point &rhs) const;
        int x{0}, y{0};
        size_t idx;
        std::optional<size_t> postIdx;
    };
    struct Line
    {
        Line(size_t idx, double length, std::pair<size_t, size_t> points);
        bool operator==(const Line &rhs) const;
        bool operator!=(const Line &rhs) const;
        size_t idx;
        double length;
        std::pair<size_t, size_t> points;
    };

  public:
    Graph() = default;
    Graph(std::string name, size_t id, const std::vector<Point> &points, std::vector<Line> lines);

    void SetName(const std::string &name);
    void SetId(size_t id);
    void SetPointsCoordinates(const std::vector<std::tuple<size_t, int, int>> &coordinates);
    void SetLines(const std::vector<Line> &lines);
    void SetPosts(const std::vector<Post> &posts);

    const std::string &GetName() const;
    size_t GetIdx() const;
    const std::vector<Point> &GetPoints() const;
    const std::vector<Line> &GetLines() const;
    size_t GetNum(size_t pointIdx) const;

  private:
    std::string name;
    size_t idx;
    std::vector<Point> points;
    std::vector<Line> lines;
    std::vector<Post> posts;
    std::unordered_map<size_t, size_t> idxToNum;
    std::unordered_map<size_t, size_t> pointToPost;
};
} // namespace RailGraph
#endif
