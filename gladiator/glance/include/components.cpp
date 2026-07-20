#include <components.hpp>
#include <chrono.hpp>
#include <iostream>

TopComponent::TopComponent(): Gtk::Box(Gtk::Orientation::VERTICAL, 0) {
    set_vexpand(true);
    set_valign(Gtk::Align::CENTER);
    set_name("topComponent");
}

std::string DateNTime::all_upper(std::string val) {
    for (char& c: val) {
        c = std::toupper(c);
    }
    return val;
}

DateNTime::DateNTime(std::string val, std::string className): Gtk::Label(all_upper(em::Chrono::get_current_values(val)), Gtk::Align::CENTER, Gtk::Align::CENTER, false) {
    em::Chrono::callback_add_minute([this]() {
        this->callback();
    });

    stringVal = val;
    set_halign(Gtk::Align::CENTER);
    set_name(className);
    set_hexpand(true);
    add_css_class("dateNTime");
}

void DateNTime::callback() {
    set_text(all_upper(em::Chrono::get_current_values(stringVal)));
}

MiddleComponent::MiddleComponent(): Gtk::Box(Gtk::Orientation::VERTICAL, 0) {
    append(*Gtk::manage(new DateNTime("%H\n%M", "time")));
    append(*Gtk::manage(new DateNTime("%a\n%b", "date")));
    append(*Gtk::manage(new DateNTime("%d\n%m", "dateNum")));

    set_vexpand(true);
    set_valign(Gtk::Align::CENTER);
    set_name("middleComponent");
}

BottomComponent::BottomComponent(): Gtk::Box(Gtk::Orientation::VERTICAL, 0) {
    set_vexpand(true);
    set_valign(Gtk::Align::CENTER);
    set_name("bottomComponent");
}