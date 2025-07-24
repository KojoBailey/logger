#ifndef KOJO_LOG_LIB
#define KOJO_LOG_LIB

#include <iostream>
#include <format>
#include <string_view>

namespace kojo {

class logger {
public:
    logger(std::string_view owner_name) : OWNER(owner_name) {}

    void show_debug(bool input) {
        m_show_debug = input;
    }
    void show_info(bool input) {
        m_show_info = input;
    }
    void show_verbose(bool input) {
        m_show_verbose = input;
    }
    void show_warn(bool input) {
        m_show_warn = input;
    }
    void show_error(bool input) {
        m_show_error = input;
    }
    void show_fatal(bool input) {
        m_show_fatal = input;
    }

    template<typename... Args>
    inline void info(Args&&... args) const {
        if constexpr (true)
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

    bool m_show_debug{false};
    bool m_show_info{false};
    bool m_show_verbose{false};
    bool m_show_warn{false};
    bool m_show_error{false};
    bool m_show_fatal{false};

    const std::string_view OWNER;
    const std::string_view LOG_FMT = "\033[{}m> [{}] {}\033[0m";

    template<typename T>
    void print(T&& content) {
        std::string output;
        output = std::regex_replace(content, std::regex("\\[0m"), std::format("[{}m", level_as_colour()));
        std::cout << std::format(LOG_FMT, level_as_colour(), level_as_str(), output) << std::endl;
    }

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

    constexpr std::string_view level_as_str() {
        switch (current_lvl) {
            case level::debug:      return "DEBUG";
            case level::info:       return "INFO";
            case level::verbose:    return "VERBOSE";
            case level::warn:       return "WARN";
            case level::error:      return "ERROR";
            case level::fatal:      return "FATAL";
        }
        return "UNKNOWN";
    }

    constexpr std::string_view level_as_colour() {
        switch (current_lvl) {
            case level::debug:      return "1;34";  // Light Blue
            case level::info:       return "0";     // Default
            case level::verbose:    return "1;34";  // Light Blue
            case level::warn:       return "1;33";  // Yellow
            case level::error:      return "1;31";  // Light Red
            case level::fatal:      return "0;31";  // Red
        }
        return "UNKNOWN";
    }
};

}

#endif // KOJO_LOG_LIB