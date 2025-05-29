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
        const auto eventNode = dailyMenu->getChildByID("event-node")->getChildByID("main-node");
        if (!dailyNode || !weeklyNode || !eventNode) return false;
        const auto colour = Mod::get()->getSettingValue<bool>("goldText") ? "goldFont.fnt" : "bigFont.fnt";
        const auto dailyStreak = Streak::calculate(GJTimedLevelType::Daily);
        const auto weeklyStreak = Streak::calculate(GJTimedLevelType::Weekly);
        const auto eventStreak = Streak::calculate(GJTimedLevelType::Event);

        m_fields->m_dailyLabel = CCLabelBMFont::create(fmt::format("Streak: {}", dailyStreak).c_str(), colour);
        m_fields->m_dailyLabel->setScale(0.3f);
        m_fields->m_dailyLabel->setAnchorPoint({0, 0});
        m_fields->m_dailyLabel->setPosition({7.5, dailyNode->getScaledContentHeight()+10});
        m_fields->m_dailyLabel->setID("streak-label"_spr);
        if (!dailyStreak) m_fields->m_dailyLabel->setVisible(false);
        dailyNode->addChild(m_fields->m_dailyLabel);

        m_fields->m_weeklyLabel = CCLabelBMFont::create(fmt::format("Streak: {}", weeklyStreak).c_str(), colour);
        m_fields->m_weeklyLabel->setScale(0.3f);
        m_fields->m_weeklyLabel->setAnchorPoint({0, 0});
        m_fields->m_weeklyLabel->setPosition({7.5, weeklyNode->getScaledContentHeight()+10});
        m_fields->m_weeklyLabel->setID("streak-label"_spr);
        if (!weeklyStreak) m_fields->m_weeklyLabel->setVisible(false);
        weeklyNode->addChild(m_fields->m_weeklyLabel);

        m_fields->m_eventLabel = CCLabelBMFont::create(fmt::format("Streak: {}", eventStreak).c_str(), colour);
        m_fields->m_eventLabel->setScale(0.3f);
        m_fields->m_eventLabel->setAnchorPoint({0, 0});
        m_fields->m_eventLabel->setPosition({7.5, eventNode->getScaledContentHeight()+10});
        m_fields->m_eventLabel->setID("streak-label"_spr);
        if (!eventStreak) m_fields->m_eventLabel->setVisible(false);
        eventNode->addChild(m_fields->m_eventLabel);
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
