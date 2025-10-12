#include <apps-revealer.hpp>
#include <gtkmm/box.h>
#include <iostream>

AppsSearcher::AppsSearcher(AppData *appData, AppsHolder &appsHolder) {
    this->appData = appData;
    this->appsHolder = &appsHolder;

    auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(sigc::mem_fun(*this, &AppsSearcher::on_key_press), false);
    appData->window->add_controller(controller);

    set_focusable();
    grab_focus();
    set_receives_default(true);
    set_can_focus(true);

    set_placeholder_text("Type to Search...");
    set_key_capture_widget(*appData->window);

    signal_search_changed().connect(sigc::mem_fun(*this, &AppsSearcher::on_search_changed));
    signal_activate().connect(sigc::mem_fun(*this, &AppsSearcher::on_activate));

    add_css_class("appsSearcher");
}

void AppsSearcher::on_activate() {
    // Catches the Enter Key event
    appsHolder->run_app();
    set_text("");
    if (appData->revealer)
        appData->revealer->set_reveal_child(false);
}

bool AppsSearcher::on_key_press(guint keyval, guint keycode, Gdk::ModifierType state) {
    int cursorPosition;
    switch (keyval) {
        case GDK_KEY_Return:
        case GDK_KEY_KP_Enter:
            // Catches Shift+Enter Key Event
            return true;
        case GDK_KEY_Tab:
            return true;
        case GDK_KEY_space:
            cursorPosition = get_position();
            set_text(get_text()+" ");
            set_position(cursorPosition+1);
            appsHolder->filter_apps(get_text());
            return true;
        case GDK_KEY_Escape:
            return true;
        default:
            return false;
    }
}

void AppsSearcher::on_search_changed() {
    appsHolder->filter_apps(get_text());
}

AppsRevealer::AppsRevealer(AppData *appData): appData(appData) {
    set_size_request(appData->screenSize.get_width()*REVEAL_WIDTH_MAX_NORM, appData->screenSize.get_height()*REVEAL_HEIGHT_NORM);
    set_transition_type(Gtk::RevealerTransitionType::SLIDE_UP);
    
    auto hoverMotion = Gtk::EventControllerMotion::create();
    add_controller(hoverMotion);

    hoverMotion->signal_enter().connect(sigc::mem_fun(*this, &AppsRevealer::on_hover_start));
    hoverMotion->signal_leave().connect(sigc::mem_fun(*this, &AppsRevealer::on_hover_stop));

    AppsHolder *appsHolder = Gtk::manage(new AppsHolder(appData));
    appsScroller = Gtk::manage(new AppsScroller(appData, *appsHolder));
    appsSearcher = Gtk::manage(new AppsSearcher(appData, *appsHolder));
    infoHolder = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 0));

    add_css_class("appsRevealer");
    infoHolder->add_css_class("infoHolder");
    infoHolder->append(*appsScroller);
    infoHolder->append(*appsSearcher);

    set_child(*infoHolder);
}

void AppsRevealer::on_hover_start(double x, double y) {
    set_reveal_child(true);
}

void AppsRevealer::on_hover_stop() {
    set_reveal_child(false);
}
