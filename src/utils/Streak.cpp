#include "Streak.hpp"

#include <Geode/utils/cocos.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Log.hpp>

using namespace geode::prelude;

std::vector<std::array<int, 2>> Streak::doubleDailies = {
    { 2983, 2984 },
    { 3095, 3096 },
    { 100431, 100432 }
};

int Streak::calculate(const GJTimedLevelType& type) {
    const auto glm = GameLevelManager::sharedState();
    auto rawData = CCDictionaryExt<gd::string, GJGameLevel*>(glm->m_dailyLevels);
    std::vector<GJGameLevel*> levels;

    for (auto [key, level] : rawData) {
        GJTimedLevelType levelType;
        if (level->m_dailyID.value() <= 100000) {
            levelType = GJTimedLevelType::Daily;
        } else if (level->m_dailyID.value() <= 200000) {
            levelType = GJTimedLevelType::Weekly;
        } else {
            levelType = GJTimedLevelType::Event;
        }

        if (levelType == type) {
            levels.push_back(level);
        }
    }

    std::ranges::sort(levels, [](GJGameLevel* a, GJGameLevel* b) {
        return a->m_dailyID.value() > b->m_dailyID.value();
    });

    auto streak = 0;
    auto lastID = 0;
    std::vector<int> skipIds;

    for (const auto& level : levels) {
        const int levelID = level->m_dailyID.value();
        if (std::ranges::find(skipIds, levelID) != skipIds.end()) {
            lastID = levelID;
            continue;
        }

        if (!lastID) {
            if (level->m_normalPercent.value() != 100) {
                lastID = levelID;
                continue;
            }

            lastID = levelID+1;
        }

        const auto completed = level->m_normalPercent.value() == 100;
        bool isNextLevel = levelID == lastID-1;
        auto pair = getDoubleDailyPair(levelID);

        if (pair.has_value()) {
            std::vector sortedIds = { pair->at(0), pair->at(1) };
            std::ranges::sort(sortedIds, std::greater<int>());

            if (sortedIds[0] == levelID) {
                if (completed) {
                    if (isNextLevel) {
                        skipIds.push_back(sortedIds[1]);
                        lastID = sortedIds[1];
                        ++streak;
                        continue;
                    }
                } else {
                    lastID = level->m_dailyID.value();
                    continue;
                }
            } else if (sortedIds[1] == levelID) {
                if (!completed) {
                    break;
                } else if (levelID == lastID-2) {
                    isNextLevel = true;
                }
            }
        }

        if (completed && isNextLevel) {
            lastID = level->m_dailyID.value();
            ++streak;
        } else break;
    }

    return streak;
}

std::optional<std::array<int, 2>> Streak::getDoubleDailyPair(const int levelID) {
    for (const auto& pair : doubleDailies) {
        if (pair[0] == levelID || pair[1] == levelID) {
            return pair;
        }
    }
    return std::nullopt;
}

void Streak::setDoubles(const std::vector<std::array<int, 2>>& doubleDailies) {
    for (const auto& pair : doubleDailies) {
        if (abs(pair[0] - pair[1]) != 1) {
            log::warn("Invalid double daily pair: {}, {}", pair[0], pair[1]);
        }
    }
    Streak::doubleDailies = doubleDailies;
}