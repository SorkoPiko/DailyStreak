#include "DailyLevelPage.hpp"
#include "../utils/Streak.hpp"

using namespace geode::prelude;

bool DSDailyLevelPage::init(const GJTimedLevelType type) {
    if (!DailyLevelPage::init(type)) return false;

    const auto streak = Streak::calculate(m_type);
    const auto streakString = fmt::format("Streak: {}", streak);
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    const auto colour = Mod::get()->getSettingValue<bool>("goldText") ? "goldFont.fnt" : "bigFont.fnt";
    float xOffset = 0.f;
    if (m_type == GJTimedLevelType::Event) xOffset = 10.f;

    m_fields->m_label = CCLabelBMFont::create(streakString.c_str(), colour);
    m_fields->m_label->setScale(0.4f);
    m_fields->m_label->setZOrder(-1);
    m_fields->m_label->setAnchorPoint({0, 0});
    m_fields->m_label->setPosition({(winSize.width / 2) - 183 + xOffset, (winSize.height / 2) + 51});
    m_fields->m_label->setID("streak-label"_spr);
    m_mainLayer->addChild(m_fields->m_label);

    if (!streak) m_fields->m_label->setVisible(false);

    return true;
}

void DSDailyLevelPage::updateLabel() {
    if (const auto streak = Streak::calculate(m_type); !streak) m_fields->m_label->setVisible(false);
    else {
        m_fields->m_label->setVisible(true);
        m_fields->m_label->setString(fmt::format("Streak: {}", streak).c_str());
    }
}