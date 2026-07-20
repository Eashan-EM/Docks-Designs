#include <iostream>
#include <gtkmm.h>

#define WIN_HEIGHT_MAX_NORM 1.00
#define WIN_WIDTH_MAX_NORM 0.025

class AppsRevealer;

struct AppData {
    std::string homeDir;
    Gdk::Rectangle screenSize;
    
    Gtk::Window *window;
};