#include <string>
#include <functional>
#include <vector>

namespace em {
    class Chrono {
        static std::vector<std::function<void()>> *callbackSecond;
        static std::vector<std::function<void()>> *callbackMinute;
        std::function<void()> callbackHour;
        std::function<void()> callbackDay;
        std::function<void()> callbackMonth;

        static bool hasRunnerStarted;
        static unsigned hour;
        static unsigned minute;
        static unsigned second;
        static unsigned day;
        static unsigned month;
        static unsigned weekDay;

        public:
            Chrono();
            static std::string get_current_values(std::string);

            bool callback_add_second(std::function<void()>);
            static bool callback_add_minute(std::function<void()>);
            bool callback_add_hour(std::function<void()>);
            bool callback_add_day(std::function<void()>);
            bool callback_add_month(std::function<void()>);
        private:
            static std::string parse_specifier(char);
            static void runner();
    };
}