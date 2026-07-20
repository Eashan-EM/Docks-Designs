#include <gtk4-layer-shell.h>
#include <window.hpp>
#include <iostream>
#include <csignal>

using namespace std;

static MainWindow *window= NULL;

MainWindow::MainWindow() {
    window = this;

    setup_app_data();
    gtk_layer_init_for_window(this->gobj());

    gtk_layer_set_layer(this->gobj(), GTK_LAYER_SHELL_LAYER_TOP);
    gtk_layer_set_exclusive_zone(gobj(), 45);
    gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_TOP, true);
    gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_LEFT, true);
    gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_BOTTOM, true);

    set_default_size(WIN_WIDTH_MAX_NORM*appData.screenSize.get_width(), WIN_HEIGHT_MAX_NORM*appData.screenSize.get_height());

    string cssPathFile = appData.homeDir+"style.css";
    set_css_style(cssPathFile);
}

void MainWindow::setup_app_data() {
    appData.window = this;
    
    appData.homeDir = "/home/em/Desktop/code/Docks/designs/gladiator/glance/";
    auto monitors = get_display()->get_monitors();
    for (int i=0; i<monitors->get_n_items(); i++) {
        auto monitor = monitors->get_typed_object<Gdk::Monitor>(i);
        monitor->get_geometry(appData.screenSize);
    }
}

void MainWindow::build_ui() {
    child = Gtk::manage(new Gtk::CenterBox());

    child->set_start_widget(*Gtk::manage(new TopComponent()));
    child->set_center_widget(*Gtk::manage(new MiddleComponent()));
    child->set_end_widget(*Gtk::manage(new BottomComponent()));
    
    child->set_hexpand(true);
    child->set_valign(Gtk::Align::FILL);

    child->set_name("mainBox");
    set_child(*child);
    show();
}

void MainWindow::set_css_style(string pathToCssFile) {
    auto cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_path(pathToCssFile);

    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(),
        cssProvider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}