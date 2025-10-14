#include <gtk4-layer-shell.h>
#include <window.hpp>
#include <iostream>
#include <csignal>

using namespace std;

static MainWindow *window= NULL;

void show_apps(int n) {
    if (window)
        window->revealer->show_apps();
}

void hide_apps(int n) {
    if (window)
        window->revealer->hide_apps();
}

MainWindow::MainWindow() {
    window = this;
    std::signal(SIGUSR1, show_apps);
    std::signal(SIGUSR2, hide_apps);

    setup_app_data();
    gtk_layer_init_for_window(this->gobj());
    gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_BOTTOM, true);
    set_default_size(WIN_WIDTH_NORM*appData.screenSize.get_width(), WIN_HEIGHT_NORM*appData.screenSize.get_height());

    string cssPathFile = appData.homeDir+"style.css";
    set_css_style(cssPathFile);
}

void MainWindow::setup_app_data() {
    appData.window = this;
    
    appData.homeDir = "/home/em/Desktop/code/Docks/designs/gladiator/app-launcher/";
    auto monitors = this->get_display()->get_monitors();
    for (int i=0; i<monitors->get_n_items(); i++) {
        auto monitor = monitors->get_typed_object<Gdk::Monitor>(i);
        monitor->get_geometry(appData.screenSize);
    }
}

void MainWindow::build_ui() {
    leftDrawingArea = Gtk::manage(new Gtk::DrawingArea());
    revealer = Gtk::manage(new AppsRevealer(&appData));
    appData.revealer = revealer;

    Gtk::Box *revealerHolder = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 0));
    revealerHolder->set_name("revealerHolder");
    revealerHolder->append(*revealer);

    rightDrawingArea = Gtk::manage(new Gtk::DrawingArea());
    child = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 0));
    
    child->append(*leftDrawingArea);
    child->append(*revealerHolder);
    child->append(*rightDrawingArea);
    child->set_vexpand(true);
    child->set_hexpand(true);

    leftDrawingArea->set_content_width(appData.screenSize.get_width()*(WIN_WIDTH_NORM-REVEAL_WIDTH_MAX_NORM)/2);
    leftDrawingArea->set_content_height(0);
    rightDrawingArea->set_content_width(appData.screenSize.get_width()*(WIN_WIDTH_NORM-REVEAL_WIDTH_MAX_NORM)/2);
    rightDrawingArea->set_content_height(0);

    leftDrawingArea->set_draw_func(sigc::mem_fun(*this, &MainWindow::on_left_draw));
    rightDrawingArea->set_draw_func(sigc::mem_fun(*this, &MainWindow::on_right_draw));

    child->set_name("topBox");
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

void MainWindow::on_left_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    if (!revealer->get_reveal_child())
        return;

    cr->set_source_rgb(0.0549, 0.0667, 0.0667);

    double radius = appData.screenSize.get_width()*(WIN_WIDTH_NORM-REVEAL_WIDTH_MAX_NORM)/2;
    double xc = (double)0.0;
    double yc = (double)height-radius;
    
    cr->begin_new_path();
    cr->move_to(xc, yc);
    cr->line_to(radius, yc);
    cr->line_to(radius, height);
    cr->line_to(xc, height);
    cr->close_path();

    cr->begin_new_sub_path();
    cr->move_to(xc, yc);
    cr->arc_negative(xc, yc, radius, 1.5*M_PI, 0.0);
    cr->close_path();

    cr->fill();
}

void MainWindow::on_right_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    if (!revealer->get_reveal_child())
        return;

    cr->set_source_rgb(0.0549, 0.0667, 0.0667);

    double radius = appData.screenSize.get_width()*(WIN_WIDTH_NORM-REVEAL_WIDTH_MAX_NORM)/2;
    double xc = (double)0.0;
    double yc = (double)height-radius;
    
    cr->begin_new_path();
    cr->move_to(radius, yc);
    cr->line_to(radius, height);
    cr->line_to(xc, height);
    cr->line_to(xc, yc);
    cr->close_path();
    
    cr->begin_new_sub_path();
    cr->move_to(radius, yc);
    cr->arc_negative(radius, yc, radius, 1.0*M_PI, 1.5*M_PI);
    cr->close_path();

    cr->fill();
}

