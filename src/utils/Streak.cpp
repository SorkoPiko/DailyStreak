#include "Streak.hpp"

using namespace geode::prelude;

int Streak::calculate(const GJTimedLevelType& type) {
    const std::vector<std::array<int, 2>> doubleDailies = {
        { 2983, 2984 },
        { 3095, 3096 }
    };

    return calculate(type, doubleDailies);
}

int Streak::calculate(const GJTimedLevelType &type, const std::vector<std::array<int, 2>>& doubleDailies) {
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

    std::sort(levels.begin(), levels.end(), [](GJGameLevel* a, GJGameLevel* b) {
        return a->m_dailyID.value() > b->m_dailyID.value();
    });

    auto streak = 0;
    auto lastID = 0;
    auto doubleDailyCompleted = false;
    auto doubleDailyFailed = false;

    for (const auto& level : levels) {
        if (!lastID) {
            if (level->m_normalPercent.value() != 100) {
                lastID = level->m_dailyID.value();
                continue;
            }

            lastID = level->m_dailyID.value()+1;
        }

        const auto completed = level->m_normalPercent.value() == 100;
        auto isNextLevel = level->m_dailyID.value() == lastID-1;

        for (const auto& doubleDaily : doubleDailies) {
            if (!isNextLevel && std::ranges::find(doubleDaily, lastID-1) != doubleDaily.end()) {
                if (doubleDailyCompleted) {
                    isNextLevel = level->m_dailyID.value() == lastID-2;
                } else if (doubleDailyFailed) {
                    break;
                }
                doubleDailyFailed = true;
            }

            if (std::ranges::find(doubleDaily, level->m_dailyID.value()) != doubleDaily.end()) {
                if (isNextLevel) {
                    lastID = level->m_dailyID.value();

                    if (completed) {
                        if (doubleDailyCompleted) continue;
                        doubleDailyCompleted = true;
                        ++streak;
                        continue;
                    }
                    if (doubleDailyFailed) break;
                    doubleDailyFailed = true;
                    continue;
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
