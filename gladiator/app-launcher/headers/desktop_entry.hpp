#include <vector>
#include <string>

enum DesktopEntryType {
    Application,
    Link,
    Directory
};

class Desktop {
    bool is_valid;
   
    public:
    class DesktopEntryGroup {
        std::vector<char*> execute_args;

        std::string _Type;
        DesktopEntryType _entryType;
        std::string _Version;
        std::string _Name;
        std::string _GenericName;
        bool _NoDisplay;
        std::string _Comment;
        std::string _Icon;
        bool _Hidden;
        std::string _TryExec;
        std::string _Exec;
        std::vector<std::string> _OnlyShowIn;
        std::vector<std::string> _NotShowIn;
        bool _DBusActivable;
        std::string _Path;
        bool _Terminal;
        std::vector<std::string> _Actions;
        std::vector<std::string> _MimeType;
        std::vector<std::string> _Categories;
        std::vector<std::string> _Implements;
        std::vector<std::string> _Keywords;
        bool _StartUpNotify;
        std::string _StartWMClass;
        std::string _URL;
        bool _PrefersNonDefaultGPU;
        bool _SingleMainWindow;

        public:
            DesktopEntryType EntryType() {return _entryType;}
            bool NoDisplay() {return _NoDisplay;}
            bool Hidden() {return _Hidden;}
            bool DBusActivable() {return _DBusActivable;}
            bool Terminal() {return _Terminal;}
            bool StartUpNotify() {return _StartUpNotify;}
            bool PrefersNonDefaultGPU() {return _PrefersNonDefaultGPU;}
            bool SingleMainWindow() {return _SingleMainWindow;}
            std::string Type() {return _Type;}
            std::string Version() {return _Version;}
            std::string Name() {return _Name;}
            std::string GenericName() {return _GenericName;}
            std::string Comment() {return _Comment;}
            std::string Icon() {return _Icon;}
            std::string TryExec() {return _TryExec;}
            std::string Exec() {return _Exec;}
            std::string Path() {return _Path;}
            std::string StartWMClass() {return _StartWMClass;}
            std::string URL() {return _URL;}
            std::vector<std::string> NotShowIn() {return _NotShowIn;}
            std::vector<std::string> OnlyShowIn() {return _OnlyShowIn;}
            std::vector<std::string> Actions() {return _Actions;}
            std::vector<std::string> MimeType() {return _MimeType;}
            std::vector<std::string> Categories() {return _Categories;}
            std::vector<std::string> Implements() {return _Implements;}
            std::vector<std::string> Keywords() {return _Keywords;}

         public:
            DesktopEntryGroup();
            bool execute_app();
            friend class DesktopEntry;
    };

    public:
        DesktopEntryGroup desktopEntryGroup;
        Desktop();
        friend class DesktopEntry;
};

class DesktopEntry {
    std::vector<std::string> directories;
    std::vector<std::string> desktopFiles;

    private:
        void parse_line(std::string, Desktop&);
        Desktop* parse_file(std::string);
        void value_fill_desktop_entry(Desktop&, std::string);
    public:
        DesktopEntry();
        std::vector<Desktop> &for_menu_strict();
        void load_all_desktop_files();
};

std::vector<std::string> split(std::string);