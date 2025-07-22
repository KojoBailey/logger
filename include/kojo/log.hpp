#ifndef KOJO_LOG_LIB
#define KOJO_LOG_LIB

#include <iostream>
#include <format>
#include <string_view>

namespace kojo {

class log {
public:
    template<typename... Args>
    void info(Args&&... args) const {
        send(level::info, std::forward<Args>(args)...);
    }

private:
    enum class level {
        debug,      // Intended for debugging purposes only.
        info,       // Standard log.
        verbose,    // More information that necessary, but potentially useful.
        warn,       // A recommendation or non-crucial alert.
        error,      // A partial, non-fatal error that allows execution to continue.
        fatal,      // A program-terminating error.
    };

    level current_lvl;

    template<typename... Args>
    void send(level lvl, std::format_string<Args...> fmt, Args&&... args) {
        current_lvl = lvl;
        print(std::format(fmt, std::forward<Args>(args)...));
    }
    template<typename T>
    void send(level lvl, T&& content) {
        current_lvl = lvl;
        print(std::forward<T>(content));
    }

    const std::string_view LOG_FMT = "\033[{}m> [{}] {}\033[0m";

    template<typename T> void print(T&& content) {
        std::string output;
        output = std::regex_replace(content, std::regex("\\[0m"), std::format("[{}m", level_as_colour()));
        std::cout << std::format(LOG_FMT, level_as_colour(), level_as_str(), output) << std::endl;
    }
};

}

#endif // KOJO_LOG_LIB