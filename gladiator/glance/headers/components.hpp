#include <gtkmm.h>
#include <iostream>

class TopComponent: public Gtk::Box {
    public:
        TopComponent();
};

class DateNTime: public Gtk::Label {
    std::string stringVal;
    public:
        DateNTime(std::string, std::string);
        void callback();
    private:
        std::string all_upper(std::string);
};

class MiddleComponent: public Gtk::Box {
    public:
        MiddleComponent();
};

class BottomComponent: public Gtk::Box {
    public:
        BottomComponent();
};