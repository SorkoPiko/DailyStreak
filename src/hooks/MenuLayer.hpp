#ifndef MENULAYER_HPP
#define MENULAYER_HPP

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class $modify(DSMenuLayer, MenuLayer) {

    struct Fields {
        Ref<CCLabelBMFont> m_dailyLabel;
        Ref<CCLabelBMFont> m_weeklyLabel;
        Ref<CCLabelBMFont> m_eventLabel;
        TaskHolder<web::WebResponse> m_listener;
    };

    static void onModify(auto& self);
    bool init();
    void updateLabel();
};

#endif
