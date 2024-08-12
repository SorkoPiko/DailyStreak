#include <Geode/modify/MenuLayer.hpp>
#include "../utils/Streak.hpp"
#include "MenuLayer.hpp"

using namespace geode::prelude;

void DSMenuLayer::onModify(auto &self) {
    self.setHookPriority("MenuLayer::init", INT_MIN/2);
}

bool DSMenuLayer::init() {
    if (!MenuLayer::init()) return false;

    if (Loader::get()->isModLoaded("ninxout.redash")) {
        const auto dailyMenu = this->getChildByID("ninxout.redash/redash-menu")->getChildByID("ninxout.redash/dailies-menu");
        const auto dailyNode = dailyMenu->getChildByID("daily-node")->getChildByID("main-node");
        const auto weeklyNode = dailyMenu->getChildByID("weekly-node")->getChildByID("main-node");
        if (!dailyNode || !weeklyNode) return false;
        const auto colour = Mod::get()->getSettingValue<bool>("goldText") ? "goldFont.fnt" : "bigFont.fnt";
        const auto dailyStreak = Streak::calculate(GJTimedLevelType::Daily);
        const auto weeklyStreak = Streak::calculate(GJTimedLevelType::Weekly);

        m_fields->m_dailyLabel = CCLabelBMFont::create(fmt::format("Streak: {}", dailyStreak).c_str(), colour);
        m_fields->m_dailyLabel->setScale(0.3f);
        m_fields->m_dailyLabel->setAnchorPoint({0, 0});
        m_fields->m_dailyLabel->setPosition({-0, dailyNode->getScaledContentHeight()});
        m_fields->m_dailyLabel->setID("streak-label"_spr);
        if (!dailyStreak) m_fields->m_dailyLabel->setVisible(false);
        dailyNode->addChild(m_fields->m_dailyLabel);

        m_fields->m_weeklyLabel = CCLabelBMFont::create(fmt::format("Streak: {}", weeklyStreak).c_str(), colour);
        m_fields->m_weeklyLabel->setScale(0.3f);
        m_fields->m_weeklyLabel->setAnchorPoint({0, 0});
        m_fields->m_weeklyLabel->setPosition({0, weeklyNode->getScaledContentHeight()});
        m_fields->m_weeklyLabel->setID("streak-label"_spr);
        if (!weeklyStreak) m_fields->m_weeklyLabel->setVisible(false);
        weeklyNode->addChild(m_fields->m_weeklyLabel);
    }

    return true;
}

void DSMenuLayer::updateLabel() {
    if (const auto streak = Streak::calculate(GJTimedLevelType::Daily); !streak) m_fields->m_dailyLabel->setVisible(false);
    else {
        m_fields->m_dailyLabel->setVisible(true);
        m_fields->m_dailyLabel->setString(fmt::format("Streak: {}", streak).c_str());
    }

    if (const auto streak = Streak::calculate(GJTimedLevelType::Weekly); !streak) m_fields->m_weeklyLabel->setVisible(false);
    else {
        m_fields->m_weeklyLabel->setVisible(true);
        m_fields->m_weeklyLabel->setString(fmt::format("Streak: {}", streak).c_str());
    }
}
