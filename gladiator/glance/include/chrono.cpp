#include <chrono.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

std::vector<std::function<void()>>* em::Chrono::callbackSecond = new std::vector<std::function<void()>>;
std::vector<std::function<void()>>* em::Chrono::callbackMinute = new std::vector<std::function<void()>>;
bool em::Chrono::hasRunnerStarted = false;
unsigned em::Chrono::hour = 0;
unsigned em::Chrono::minute = 0;
unsigned em::Chrono::second = 0;
unsigned em::Chrono::day = 0;
unsigned em::Chrono::month = 0;
unsigned em::Chrono::weekDay = 0;

em::Chrono::Chrono() {

}

void em::Chrono::runner() {
    unsigned newHour=hour, newMinute=minute, newMonth=month, newDay=day, newSecond=second;

    std::thread t([&newSecond, &newMinute]() {
        while(true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            newSecond++;
            if (newSecond>=60) {
                newSecond = 0;
                newMinute++;
                for (const auto callback: *callbackMinute)
                    callback();
            }
        }
    });

    hasRunnerStarted = true;
    t.detach();
}

std::string em::Chrono::parse_specifier(char c) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* local_tm = std::localtime(&now_c);
    hour = local_tm->tm_hour;
    minute = local_tm->tm_min;
    day = local_tm->tm_mday;
    weekDay = local_tm->tm_wday;
    month = local_tm->tm_mon;
    std::string daysOfWeek[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    std::string monthsName[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

    std::string ret;
    switch (c) {
        case 'a':
            ret += daysOfWeek[weekDay].substr(0, 3);
            break;
        case 'b':
            ret += monthsName[month].substr(0, 3);
            break;
        case 'd':
            if (day<10)
                ret += '0';
            ret += std::to_string(day);
            break;
        case 'H':
            if (hour<10)
                ret += '0';
            ret += std::to_string(hour);
            break;
        case 'm':
            month++;
            if (month<10)
                ret += '0';
            ret += std::to_string(month);
            break;
        case 'M':
            if (minute<10)
                ret += '0';
            ret += std::to_string(minute);
            break;
    }
    return ret;
}

std::string em::Chrono::get_current_values(std::string ask) {
    std::string ret;
    for (int i=0; i<ask.size(); i++) {
        if (ask[i]=='%' && (i+1)<ask.size()) {
            ret += parse_specifier(ask[++i]);
            continue;
        }
        ret += ask[i];
    }
    return ret;
}

bool em::Chrono::callback_add_minute(std::function<void()> callback) {
    if (!hasRunnerStarted) {
        runner();
    }
    callbackMinute->push_back(callback);
    return true;
}