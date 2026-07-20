#include <iostream>
#include <app-data.hpp>
#include <components.hpp>
#include <gtkmm.h>

class MainWindow: public Gtk::Window {
    Gtk::CenterBox *child;
    AppData appData;
    
    public:
        MainWindow();
        void setup_app_data();
        void build_ui();
        void set_css_style(std::string);
};

