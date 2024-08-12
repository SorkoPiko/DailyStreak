#include "Streak.hpp"

using namespace geode::prelude;

int Streak::calculate(const GJTimedLevelType &type) {
    const auto glm = GameLevelManager::sharedState();
    auto rawData = CCDictionaryExt<gd::string, GJGameLevel*>(glm->m_dailyLevels);
    std::vector<GJGameLevel*> levels;

    for (auto [key, level] : rawData) {
        if (const auto& levelType = level->m_dailyID.value() < 100000 ? GJTimedLevelType::Daily : GJTimedLevelType::Weekly; levelType == type) {
            levels.push_back(level);
        }
    }

    std::sort(levels.begin(), levels.end(), [](GJGameLevel* a, GJGameLevel* b) {
        return a->m_dailyID.value() > b->m_dailyID.value();
    });

    auto streak = 0;
    auto lastID = 0;

    for (const auto& level : levels) {
        if (!lastID) {
            if (level->m_normalPercent.value() != 100) {
                lastID = level->m_dailyID.value();
                continue;
            }

            lastID = level->m_dailyID.value()+1;
        }

        if (level->m_normalPercent.value() == 100 && level->m_dailyID.value() == lastID-1) {
            lastID = level->m_dailyID.value();
            ++streak;
        } else break;
    }

    return streak;
}