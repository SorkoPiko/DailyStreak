#ifndef STREAK_HPP
#define STREAK_HPP

using namespace geode::prelude;

class Streak {
public:
    static int calculate(const GJTimedLevelType& type);
    static int calculate(const GJTimedLevelType& type, const std::vector<std::array<int, 2>>& doubleDailies);
};

#endif
