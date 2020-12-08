#include "parser.h"

namespace RailGraph
{
Graph GraphParser::GetGraph() const
{
    std::string name = document["name"].GetString();
    size_t id = document["idx"].GetUint();

    std::vector<Graph::Point> points;
    for (const auto &point : document["points"].GetArray())
    {
        if (point["post_idx"].IsUint())
        {
            points.emplace_back(point["idx"].GetUint(), point["post_idx"].GetUint());
        }
        else
        {
            points.emplace_back(point["idx"].GetUint());
        }
    }

    std::vector<Graph::Line> lines;
    for (const auto &line : document["lines"].GetArray())
    {
        lines.emplace_back(line["idx"].GetUint(), line["length"].GetDouble(),
                           std::make_pair<size_t, size_t>(line["points"][0].GetUint(), line["points"][1].GetUint()));
    }

    return Graph(name, id, points, lines);
}

Graph ParseMap0FromJson(const std::string &parseText)
{
    rapidjson::Document document;
    document.Parse(parseText.c_str());

    std::string name = document["name"].GetString();
    size_t id = document["idx"].GetUint();

    std::vector<Graph::Point> points;
    for (const auto &point : document["points"].GetArray())
    {
        if (point["post_idx"].IsUint())
        {
            points.emplace_back(point["idx"].GetUint(), point["post_idx"].GetUint());
        }
        else
        {
            points.emplace_back(point["idx"].GetUint());
        }
    }

    std::vector<Graph::Line> lines;
    for (const auto &line : document["lines"].GetArray())
    {
        lines.emplace_back(line["idx"].GetUint(), line["length"].GetDouble(),
                           std::make_pair<size_t, size_t>(line["points"][0].GetUint(), line["points"][1].GetUint()));
    }

    return Graph(name, id, points, lines);
}

std::vector<Graph::Post> ParseMap1FromJson(const std::string &parseText)
{
    rapidjson::Document document;
    document.Parse(parseText.c_str());
    std::vector<Graph::Post> result;

    for (const auto &post : document["posts"].GetArray())
    {
        switch (post["type"].GetInt())
        {
        case 1: {
            result.emplace_back(Graph::Info{.idx = static_cast<size_t>(post["idx"].GetInt()),
                                            .point_idx = static_cast<size_t>(post["point_idx"].GetInt()),
                                            .name = post["name"].GetString()},
                                Graph::CityInfo{.armor = post["armor"].GetInt(),
                                                .armorCapacity = post["armor_capacity"].GetInt(),
                                                .level = post["level"].GetInt(),
                                                .nextLevelPrice = post["next_level_price"].IsNull() ?
                                                                  std::nullopt :
                                                                  std::optional(post["next_level_price"].GetInt()),
                                                .population = post["population"].GetInt(),
                                                .populationCapacity = post["population_capacity"].GetInt(),
                                                .product = post["product"].GetInt(),
                                                .productCapacity = post["product_capacity"].GetInt(),
                                                .trainCooldown = post["train_cooldown"].GetInt(),
                                                .playerIdx = post["player_idx"].IsNull() ?
                                                             std::nullopt :
                                                             std::optional(post["player_idx"].GetString())});
            break;
        }
        case 2: {
            result.emplace_back(Graph::Info{.idx = static_cast<size_t>(post["idx"].GetInt()),
                                            .point_idx = static_cast<size_t>(post["point_idx"].GetInt()),
                                            .name = post["name"].GetString()},
                                Graph::MarketInfo{.product = post["product"].GetInt(),
                                                  .productCapacity = post["product_capacity"].GetInt(),
                                                  .replenishment = post["replenishment"].GetInt()});
            break;
        }
        case 3: {
            result.emplace_back(Graph::Info{.idx = static_cast<size_t>(post["idx"].GetInt()),
                                            .point_idx = static_cast<size_t>(post["point_idx"].GetInt()),
                                            .name = post["name"].GetString()},
                                Graph::StorageInfo{.armor = post["armor"].GetInt(),
                                                   .armorCapacity = post["armor_capacity"].GetInt(),
                                                   .replenishment = post["replenishment"].GetInt()});
            break;
        }
        }
    }
    return result;
}

std::vector<std::tuple<size_t, int, int>> ParseMap10FromJson(const std::string &parseText)
{
    rapidjson::Document document;
    document.Parse(parseText.c_str());
    std::vector<std::tuple<size_t, int, int>> result;

    for (const auto &coordinate : document["coordinates"].GetArray())
    {
        result.emplace_back(coordinate["idx"].GetInt(), coordinate["x"].GetInt(), coordinate["y"].GetInt());
    }

    return result;
}

} // namespace RailGraph
