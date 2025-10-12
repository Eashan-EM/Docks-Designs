#include <apps-revealer.hpp>
#include <regex>

AppsHolder::AppsHolder(AppData aD): Gtk::Box(Gtk::Orientation::VERTICAL, 20) {
    appData = aD;
    add_apps();    
}

void AppsHolder::add_apps() {
    apps = entry.for_menu_strict();

    Gtk::Box *appHolder;
    Gtk::Box *appIconName;
    Gtk::Image *appIcon;
    Gtk::Label *appName;
    int length;
    
    for (Desktop app: apps) {
        auto hoverMotion = Gtk::EventControllerMotion::create();
        showableApps.push_back(Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL)));
        length = showableApps.size();
        appHolder = showableApps[length-1];

        appHolder->append(*(appIconName = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 10))));
        appIconName->append(*(appIcon = Gtk::manage(new Gtk::Image(Gio::Icon::create(app.desktopEntryGroup.Icon())))));
        appIconName->append(*(appName = Gtk::manage(new Gtk::Label(app.desktopEntryGroup.Name()))));

        appHolder->add_controller(hoverMotion);
        
        hoverMotion->signal_enter().connect(sigc::bind(sigc::mem_fun(*this, &AppsHolder::on_app_hover_start), length-1));
        appHolder->set_vexpand(false);
        appHolder->set_hexpand(true);
        appIcon->set_icon_size(Gtk::IconSize::LARGE);
        appIconName->set_vexpand(false);

        appName->add_css_class("appName");
        appIconName->add_css_class("appIconName");
        appHolder->add_css_class("appHolderBox");
    }
}

void AppsHolder::show_apps() {
    for (Gtk::Widget *wid: showableApps) {
        wid->set_visible(true);
        wid->add_css_class("notSelectedApp");
        append(*wid);
    }
    appData.selectedApp = 0;
}

bool case_insensitive_partial_match(const std::string& main_string, const std::string& search)
{
    if (search.empty())
        return true;

    std::string main_lower = main_string;
    std::string search_lower = search;

    std::transform(main_lower.begin(), main_lower.end(), main_lower.begin(), 
                   [](unsigned char c){ return std::tolower(c); });
    
    std::transform(search_lower.begin(), search_lower.end(), search_lower.begin(), 
                   [](unsigned char c){ return std::tolower(c); });

    return main_lower.find(search_lower) != std::string::npos;
}

void AppsHolder::filter_apps(std::string app) {
    Gtk::Widget *wid;
    for (int i=0; i<apps.size(); i++) {
        wid = showableApps[i];
        wid->set_visible(match_app(apps[i], app));
    }
    bool found = false;
    for (int i=0; i<showableApps.size(); i++) {
        wid = showableApps[i];
        if (found) {
            wid->set_name("notSelected");
            wid->add_css_class("notSelectedApp");
        }
        if (wid->get_visible() && !found) {
            wid->remove_css_class("notSelectedApp");
            wid->set_name("selectedApp");
            appData.selectedApp = i;
            found = true;
        }
    }
}

bool AppsHolder::match_app(Desktop desktop, std::string app) {
    bool ret = false;
    ret = case_insensitive_partial_match(desktop.desktopEntryGroup.Name(), app);
    return ret;
}

void AppsHolder::run_app() {
    apps[appData.selectedApp].desktopEntryGroup.execute_app();
}

void AppsHolder::on_app_hover_start(double x, double y, int appId) {
    if (appId<showableApps.size()) {
        if (appData.selectedApp<showableApps.size()) {
            showableApps[appData.selectedApp]->set_name("notSelected");
            showableApps[appData.selectedApp]->add_css_class("notSelectedApp");
        }
        showableApps[appId]->remove_css_class("notSelectedApp");
        showableApps[appId]->set_name("selectedApp");
        appData.selectedApp = appId;
    }
}