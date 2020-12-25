#ifndef RAIL_SIMULATOR_GRAPH_H
#define RAIL_SIMULATOR_GRAPH_H
#include <cmath>
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
        Post() = default;
        Post(Info info, std::variant<MarketInfo, CityInfo, StorageInfo> postInfo);
        Info info;

        typedef std::variant<MarketInfo, CityInfo, StorageInfo> PostInfo;
        static int MarketIndex, CityIndex, StorageIndex;
        PostInfo postInfo;
    };
    struct TrainInfo
    {
        int cooldown;
        int fuel, fuelCapacity, fuelConsumption;
        int goods, goodsCapacity;
        std::optional<int> goodsType;
        int idx, lineIdx;
        int level;
        std::optional<int> nextLevelPrice;
        std::optional<std::string> playerIdx;
        int position, speed;
    };
    struct Train
    {
        explicit Train(TrainInfo info);
        TrainInfo info;
    };
    struct Rating
    {
        std::string idx;
        std::string name;
        int rating;
        std::string town;
    };
    struct Point
    {
        Point() = default;
        explicit Point(size_t idx, std::optional<size_t> postIdx = std::nullopt);
        explicit Point(float x, float y, float renderX, float renderY);
        bool operator==(const Point &rhs) const;
        bool operator!=(const Point &rhs) const;
        float renderX{0}, renderY{0};
        float x{0}, y{0};
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
    void SetTrains(const std::vector<Train> &trains);
    void SetRatings(const std::vector<Rating> &ratings);

    const std::string &GetName() const;
    size_t GetIdx() const;
    std::vector<Point> &GetPoints();
    const std::vector<Line> &GetLines() const;
    std::vector<Post> GetPosts() const;
    std::vector<Train> GetTrains() const;
    std::string GetPointInfo(int index) const;
    std::string GetTrainInfo(int index) const;
    size_t GetNum(size_t pointIdx) const;
    static float GetDist(const Point &first, const Point &second);
    static float GetRenderDist(const Point &first, const Point &second);
    std::vector<Rating> GetRatings() const;

  private:
    std::string name;
    size_t idx;
    std::vector<Point> points;
    std::vector<Line> lines;
    std::vector<Post> posts;
    std::vector<Train> trains;
    std::vector<Rating> ratings;
    std::unordered_map<size_t, size_t> idxToNum;
    std::unordered_map<size_t, size_t> pointToPost;
};
} // namespace RailGraph
#endif
