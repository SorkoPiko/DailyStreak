#ifndef MENULAYER_HPP
#define MENULAYER_HPP

#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(DSMenuLayer, MenuLayer) {

    struct Fields {
        Ref<CCLabelBMFont> m_dailyLabel;
        Ref<CCLabelBMFont> m_weeklyLabel;
    };

    static void onModify(auto& self);
    bool init();
    void updateLabel();
};

#endif
