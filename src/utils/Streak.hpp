#ifndef STREAK_HPP
#define STREAK_HPP

#include <Geode/Prelude.hpp>
#include <Geode/Enums.hpp>
#include <optional>
#include <vector>

using namespace geode::prelude;

class Streak {
protected:
    static std::vector<std::array<int, 2>> doubleDailies;

    static std::optional<std::array<int, 2>> getDoubleDailyPair(int levelID);

public:
    static int calculate(const GJTimedLevelType &type);
    static void setDoubles(const std::vector<std::array<int, 2>> &doubleDailies);
};

#endif
