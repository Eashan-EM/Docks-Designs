#include <iostream>
#include <apps-revealer.hpp>
#include <gtkmm.h>

class MainWindow: public Gtk::Window {
    Gtk::DrawingArea *leftDrawingArea;
    AppsRevealer *revealer;
    Gtk::DrawingArea *rightDrawingArea;
    Gtk::Box *child;
    AppData appData;
    public:
        MainWindow();
        void setup_app_data();
        void build_ui();
        void set_css_style(std::string);
        void on_left_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
        void on_right_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};

