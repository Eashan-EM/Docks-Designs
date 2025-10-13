#include <gtkmm/revealer.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/scrolledwindow.h>
#include <app-data.hpp>
#include <desktop_entry.hpp>

#define APPS_REVEALER

class AppsRevealer;

class AppsHolder: public Gtk::Box {
    AppData *appData;
    DesktopEntry entry;
    std::vector<Desktop> apps;
    std::vector<Gtk::Box*> showableApps;
    AppsRevealer *appsRevealer;

    bool match_app(Desktop, std::string);
    public:
        AppsHolder(AppData*, AppsRevealer*);
        void add_apps();
        void show_apps();
        void on_app_hover_start(double, double, int);
        void on_app_click(int, double, double);
        void filter_apps(std::string);
        void run_app();
};

class AppsScroller: public Gtk::ScrolledWindow {
    AppData *appData;
    Gtk::Stack *stack;
    AppsHolder *child;
    public:
        AppsScroller(AppData*, AppsHolder&);
};

class AppsSearcher: public Gtk::SearchEntry {
    AppData *appData;
    AppsHolder *appsHolder;
    public:
        Gtk::Entry *entry;
        AppsSearcher(AppData*, AppsHolder&);
        void on_search_changed();
        bool on_key_press(guint, guint, Gdk::ModifierType);
        void on_activate();
        void run_app();
};

class AppsRevealer: public Gtk::Revealer {
    AppsScroller *appsScroller;
    AppsSearcher *appsSearcher;
    Gtk::Box *infoHolder;
    AppData *appData;
    public:
        AppsRevealer(AppData*);
        void on_hover_start(double, double);
        void on_hover_stop();
        void clear_search_entry();
};

bool case_insensitive_partial_match(const std::string&, const std::string&);