#include <gtkmm.h>
#include <window.hpp>

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create("org.em.docks");

    app->wrap_in_search_entry2(true);

    app->signal_activate().connect([app]() {
        MainWindow* window = Gtk::manage(new MainWindow());
        app->add_window(*window);
        window->build_ui();
    });

    return app->run(argc, argv);
}