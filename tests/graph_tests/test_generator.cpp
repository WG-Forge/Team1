#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

const int kPoints = 50;

int GetRand(int l, int r)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(l, r);

    return distrib(gen);
}

class Dsu
{
  private:
    std::vector<int> parent;
    int size{};
    int components{};

  public:
    Dsu() = default;

    explicit Dsu(int size) : size(size)
    {
        components = size;
        parent.resize(size + 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int GetParent(int v)
    {
        if (parent[v] == v)
        {
            return parent[v];
        }
        return parent[v] = GetParent(parent[v]);
    }

    void Unite(int u, int v)
    {
        int parent_u = GetParent(u);
        int parent_v = GetParent(v);

        if (parent_u == parent_v)
        {
            return;
        }

        if (GetRand(0, 1) == 1)
        {
            std::swap(parent_u, parent_v);
        }

        parent[parent_u] = parent_v;
        components--;
    }

    int GetComponents() const
    {
        return components;
    }
};

int main()
{
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

    rapidjson::Value points(rapidjson::kArrayType);

    for (int i = 0; i < kPoints; ++i)
    {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("idx", i + 1, allocator);
        object.AddMember("post_idx", rapidjson::Value(), allocator);
        points.PushBack(object, allocator);
    }

    document.AddMember("name", "test", allocator);
    document.AddMember("points", points, allocator);

    Dsu dsu(kPoints);
    std::vector<std::pair<int, int>> edges;
    for (int i = 1; i <= kPoints; ++i)
    {
        for (int j = i + 1; j <= kPoints; ++j)
        {
            edges.emplace_back(i, j);
        }
    }
    shuffle(edges.begin(), edges.end(), std::mt19937(std::random_device()()));

    rapidjson::Value lines(rapidjson::kArrayType);

    for (size_t i = 0; i < edges.size(); ++i)
    {
        int u = edges[i].first, v = edges[i].second;
        dsu.Unite(u, v);
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("idx", i + 1, allocator);
        object.AddMember("length", GetRand(1, 1), allocator);
        rapidjson::Value pointsObject(rapidjson::kArrayType);
        pointsObject.PushBack(u, allocator);
        pointsObject.PushBack(v, allocator);
        object.AddMember("points", pointsObject, allocator);
        lines.PushBack(object, allocator);
        if (dsu.GetComponents() == 1)
        {
            break;
        }
    }

    document.AddMember("lines", lines, allocator);
    document.AddMember("idx", 1, allocator);

    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    document.Accept(writer);

    std::ofstream out("../tests/graph_tests/test_graph_1.json");

    out << strbuf.GetString() << std::endl;

    out.close();

    return 0;
}