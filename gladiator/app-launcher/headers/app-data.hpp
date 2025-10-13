#include <iostream>
#include <gtkmm.h>

#define WIN_HEIGHT_NORM 0.01
#define WIN_HEIGHT_MAX_NORM 0.50
#define WIN_WIDTH_NORM 0.35
#define REVEAL_HEIGHT_NORM 0.01
#define REVEAL_WIDTH_MAX_NORM 0.28

struct AppData {
    std::string homeDir;
    Gdk::Rectangle screenSize;
    
    Gtk::Window *window;
    Gtk::Revealer *revealer;
    int selectedApp;
};