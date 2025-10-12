#include <desktop_entry.hpp>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <bits/stdc++.h>
#include <unistd.h>

Desktop::Desktop() {
}

Desktop::DesktopEntryGroup::DesktopEntryGroup() {

}

bool Desktop::DesktopEntryGroup::execute_app() {
    pid_t pid = fork();
    if (pid<0) {
        return false;
    } else if (pid==0) {
        execvp(execute_args[0], execute_args.data());
    }
    return true;
}

DesktopEntry::DesktopEntry() {
    std::string root = "/";
    std::string home = std::string(getenv("HOME"));

    directories.push_back(root+"usr/share/applications/");
    directories.push_back(root+"usr/local/share/applications/");
    directories.push_back(home+"/.local/share/applications/");
    load_all_desktop_files();
}

void DesktopEntry::load_all_desktop_files() {
    namespace fs = std::filesystem;
    for (std::string directory: directories) {
        if (fs::exists(directory) && fs::is_directory(directory)) {
            for (const auto& entry: fs::directory_iterator(directory)) {
                if (fs::is_regular_file(entry.path()) && entry.path().string().contains(".desktop")) {
                    desktopFiles.push_back(entry.path().string());
                }
            }
        }
    }
}

std::vector<Desktop> &DesktopEntry::for_menu_strict() {
    std::vector<Desktop> *desktops = new std::vector<Desktop>;
    Desktop *desktop;
    for (std::string file: desktopFiles) {
        desktop = parse_file(file);
        desktops->push_back(*desktop);
    }
    return *desktops;
}

Desktop* DesktopEntry::parse_file(std::string path) {
    Desktop *desktop = new Desktop();
    std::ifstream inputFile(path);

    if (inputFile.is_open()) {
        std::string line;

        while (std::getline(inputFile, line)) {
            parse_line(line, *desktop);
        }
        desktop->is_valid = true;
    } else {
        desktop->is_valid = false;
    }
    return desktop;
}

void DesktopEntry::parse_line(std::string line, Desktop &desktop) {
    std::regex comment(R"(\s*#)");
    std::regex desktopEntryGroup(R"(^\[Desktop Entry\]$)");
    std::regex isGroup(R"(^\[.*\]$)");
    std::regex isValue(R"(.*=.*)");
    bool inGroup;
    bool inDesktopEntryGroup = true;
    if (std::regex_match(line, isGroup)) {
        inGroup = true;
        inDesktopEntryGroup = (std::regex_match(line, desktopEntryGroup))?true:false;
    } else if (std::regex_match(line, isValue)) {
        if (inDesktopEntryGroup) {
            value_fill_desktop_entry(desktop, line);
        }
    }
}

void DesktopEntry::value_fill_desktop_entry(Desktop &desktop, std::string line) {
    std::string key;
    std::string value;
    std::stringstream ss(line);
    char delm = '=';
    getline(ss, key, delm);
    getline(ss, value, delm);

    if (key=="Type") {
        desktop.desktopEntryGroup._Type = value;
        if (value=="Application")
            desktop.desktopEntryGroup._entryType = DesktopEntryType::Application;
        else if (value=="Directory")
            desktop.desktopEntryGroup._entryType = DesktopEntryType::Directory;
        else if (value=="Link")
            desktop.desktopEntryGroup._entryType = DesktopEntryType::Link;
    } else if (key=="Version")
        desktop.desktopEntryGroup._Version = value;
    else if (key=="Name")
        desktop.desktopEntryGroup._Name = value;
    else if (key=="GenericName")
        desktop.desktopEntryGroup._GenericName = value;
    else if (key=="NoDisplay")
        desktop.desktopEntryGroup._Version = (value=="true")?true:false;
    else if (key=="Comment")
        desktop.desktopEntryGroup._Comment = value;
    else if (key=="Icon")
        desktop.desktopEntryGroup._Icon = value;
    else if (key=="Hidden")
        desktop.desktopEntryGroup._Version = (value=="true")?true:false;
    else if (key=="NotShowIn")
        desktop.desktopEntryGroup._NotShowIn = split(value);
    else if (key=="OnlyShowIn")
        desktop.desktopEntryGroup._OnlyShowIn = split(value);
    else if (key=="DBusActivatable")
        desktop.desktopEntryGroup._DBusActivable = (value=="true")?true:false;
    else if (key=="TryExec")
        desktop.desktopEntryGroup._TryExec = value;
    else if (key=="Exec") {
        std::regex special(R"(^%.*)");
        std::stringstream ss(value);
        std::string *temp = new std::string();
        char delm = ' ';
        while (getline(ss, *temp, delm)) {
            if (!std::regex_match(*temp, special)) 
                desktop.desktopEntryGroup.execute_args.push_back(const_cast<char*>(temp->c_str()));
            temp = new std::string();
        }
        desktop.desktopEntryGroup.execute_args.push_back(NULL);
        desktop.desktopEntryGroup._Exec = value;
    } else if (key=="Path")
        desktop.desktopEntryGroup._Path = value;
    else if (key=="Terminal")
        desktop.desktopEntryGroup._Terminal = (value=="true")?true:false;
    else if (key=="Actions")
        desktop.desktopEntryGroup._Actions = split(value);
    else if (key=="MimeType")
        desktop.desktopEntryGroup._MimeType = split(value);
    else if (key=="Categories")
        desktop.desktopEntryGroup._Categories = split(value);
    else if (key=="Implements")
        desktop.desktopEntryGroup._Implements = split(value);
    else if (key=="Keywords")
        desktop.desktopEntryGroup._Keywords = split(value);
    else if (key=="StartupNotify")
        desktop.desktopEntryGroup._StartUpNotify = (value=="true")?true:false;
    else if (key=="StartupWMClass")
        desktop.desktopEntryGroup._StartWMClass = value;
    else if (key=="URL")
        desktop.desktopEntryGroup._URL = value;
    else if (key=="PrefersNonDefaultGPU")
        desktop.desktopEntryGroup._PrefersNonDefaultGPU = (value=="true")?true:false;
    else if (key=="SingleMainWindow")
        desktop.desktopEntryGroup._SingleMainWindow = (value=="true")?true:false;
}

std::vector<std::string> split(std::string line) {
    std::vector<std::string> *ret = new std::vector<std::string>;

    std::stringstream ss(line);
    std::string temp;
    char delm = ';';
    while (getline(ss, temp, delm))
        ret->push_back(temp);
    return *ret;
}
