#ifndef DAILYLEVELPAGE_HPP
#define DAILYLEVELPAGE_HPP

#include <Geode/modify/DailyLevelPage.hpp>

using namespace geode::prelude;

class $modify(DSDailyLevelPage, DailyLevelPage) {

    struct Fields {
        Ref<CCLabelBMFont> m_label;
    };

    bool init(GJTimedLevelType type);

    void updateLabel();
};

#endif
