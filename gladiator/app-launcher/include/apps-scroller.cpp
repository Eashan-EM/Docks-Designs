#include <apps-revealer.hpp>
#include <iostream>

AppsScroller::AppsScroller(AppData *appData, AppsHolder &appsHolder): appData(appData) {
    add_css_class("appsScroller");
    set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);

    child = &appsHolder;
    child->show_apps();
    child->filter_apps("");

    set_child(*child);
    
    set_min_content_height(WIN_HEIGHT_MAX_NORM*appData->screenSize.get_height());
    set_kinetic_scrolling(true);
}