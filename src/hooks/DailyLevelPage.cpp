#include <Geode/modify/DailyLevelPage.hpp>

using namespace geode::prelude;

class $modify(DSDailyLevelPage, DailyLevelPage) {

    struct Fields {
        Ref<CCLabelBMFont> m_label;
    };

    bool init(const GJTimedLevelType type) {
        if (!DailyLevelPage::init(type)) return false;

        const auto streak = fmt::format("Streak: {}", calculateStreak(type));

        m_fields->m_label = CCLabelBMFont::create(streak.c_str(), "goldFont.fnt");
        m_fields->m_label->setAnchorPoint({0.5, 0});
        m_fields->m_label->setPosition({getScaledContentWidth()/2, 5.f});
        m_fields->m_label->setZOrder(9);
        m_fields->m_label->setID("streak-label"_spr);
        m_mainLayer->addChild( m_fields->m_label);

        return true;
    }

    static int calculateStreak(const GJTimedLevelType type) {
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
};