#include <Geode/modify/DailyLevelNode.hpp>
#include "DailyLevelPage.hpp"

using namespace geode::prelude;

class $modify(DSDailyLevelNode, DailyLevelNode) {
    void onClaimReward(CCObject* sender) {
        DailyLevelNode::onClaimReward(sender);
        typeinfo_cast<DSDailyLevelPage*>(m_page)->updateLabel();
    }
};