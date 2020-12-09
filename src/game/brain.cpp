#include "brain.h"
Brain::Brain(RailGraph::Graph &map) : map(map)
{
    townLevelInformation = {{{"Population Capacity", 1}, std::optional<int>(10)},
                            {{"Product Capacity", 1}, std::optional<int>(200)},
                            {{"Armor Capacity", 1}, std::optional<int>(200)},
                            {{"Cooldown After Crash", 1}, std::optional<int>(2)},
                            {{"Next Level Price", 1}, std::optional<int>(100)},
                            {{"Population Capacity", 2}, std::optional<int>(20)},
                            {{"Product Capacity", 2}, std::optional<int>(500)},
                            {{"Armor Capacity", 2}, std::optional<int>(500)},
                            {{"Cooldown After Crash", 2}, std::optional<int>(1)},
                            {{"Next Level Price", 2}, std::optional<int>(200)},
                            {{"Population Capacity", 3}, std::optional<int>(40)},
                            {{"Product Capacity", 3}, std::optional<int>(10000)},
                            {{"Armor Capacity", 3}, std::optional<int>(10000)},
                            {{"Cooldown After Crash", 3}, std::optional<int>(0)},
                            {{"Next Level Price", 3}, std::optional<int>(std::nullopt)}};
    trainLevelInformation = {
        {{"Goods Capacity", 1}, std::optional<int>(40)},  {{"Fuel Capacity", 1}, std::optional<int>(400)},
        {{"Fuel Consumption", 1}, std::optional<int>(1)}, {{"Next Level Price", 1}, std::optional<int>(40)},
        {{"Goods Capacity", 2}, std::optional<int>(80)},  {{"Fuel Capacity", 2}, std::optional<int>(800)},
        {{"Fuel Consumption", 2}, std::optional<int>(1)}, {{"Next Level Price", 2}, std::optional<int>(80)},
        {{"Goods Capacity", 3}, std::optional<int>(160)}, {{"Fuel Capacity", 3}, std::optional<int>(1600)},
        {{"Fuel Consumption", 3}, std::optional<int>(1)}, {{"Next Level Price", 3}, std::optional<int>(std::nullopt)}};
}
