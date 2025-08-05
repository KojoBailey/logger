#ifndef KOJO_LOG_LIB
#define KOJO_LOG_LIB

#include <iostream>
#include <filesystem>
#include <format>
#include <source_location>
#include <string_view>

namespace kojo {

class logger {
public:
    enum class level {
        debug,      // Intended for debugging purposes only.
        info,       // Standard log.
        verbose,    // More information that necessary, but potentially useful.
        warn,       // A recommendation or non-crucial alert.
        error,      // A partial, non-fatal error that allows execution to continue.
        fatal,      // A program-terminating error.
    };

    enum class status {
        ok = 0,
        null_file,
        file_magic,
        version,
        null_pointer,
        type_mismatch,
        bad_value,
        null_data,
        missing_field,
        value_mismatch,
    };

    bool show_debug;
    bool show_verbose;
    bool show_info;
    bool show_warn;
    bool show_error;
    bool show_fatal;

    explicit logger(std::string_view owner_name,
            bool _show_debug = false, 
            bool _show_verbose = false,
            bool _show_info = true,
            bool _show_warn = true,
            bool _show_error = true,
            bool _show_fatal = true
    ) : owner(owner_name),
        show_debug(_show_debug),
        show_verbose(_show_verbose),
        show_info(_show_info),
        show_warn(_show_warn),
        show_error(_show_error),
        show_fatal(_show_fatal)
    {}

    template<typename... Args>
    inline void debug(std::string_view msg, const std::source_location& loc = std::source_location::current()) {
        if (!show_debug) return;
        src = get_src_info(loc);
        std::cout << std::format("\033[{}m> [{}; {}] [{}] {}\033[0m\n",
            level_color(level::debug),
            owner,
            src,
            level_string(level::debug),
            msg
        );
    }
    template<typename... Args>
    inline void info(std::string_view msg, const std::source_location& loc = std::source_location::current()) {
        if (!show_info) return;
        src = get_src_info(loc);
        std::cout << std::format("\033[{}m> [{}; {}] [{}] {}\033[0m\n",
            level_color(level::info),
            owner,
            src,
            level_string(level::info),
            msg
        );
    }
    template<typename... Args>
    inline void verbose(std::string_view msg, const std::source_location& loc = std::source_location::current()) {
        if (!show_verbose) return;
        src = get_src_info(loc);
        std::cout << std::format("\033[{}m> [{}; {}] [{}] {}\033[0m\n",
            level_color(level::verbose),
            owner,
            src,
            level_string(level::verbose),
            msg
        );
    }

    template<typename... Args>
    inline void warn(
        status _status, 
        std::string_view details, 
        std::string_view suggestion, 
        const std::source_location& loc = std::source_location::current()
    ) {
        if (!show_warn) return;
        src = get_src_info(loc);
        std::string msg = std::format("code {:03}: {}\n\t{}\n\t{}", static_cast<int>(_status), status_string(_status), details, suggestion);
        std::cout << std::format("\033[{}m> [{}; {}] [{}] {}\033[0m\n",
            level_color(level::warn),
            owner,
            src,
            level_string(level::warn),
            msg
        );
    }
    template<typename... Args>
    inline void error(
        status _status, 
        std::string_view details, 
        std::string_view suggestion, 
        const std::source_location& loc = std::source_location::current()
    ) {
        if (!show_error) return;
        src = get_src_info(loc);
        std::string msg = std::format("code {:03}: {}\n\t{}\n\t{}", static_cast<int>(_status), status_string(_status), details, suggestion);
        std::cout << std::format("\033[{}m> [{}; {}] [{}] {}\033[0m\n",
            level_color(level::error),
            owner,
            src,
            level_string(level::error),
            msg
        );
    }
    template<typename... Args>
    inline void fatal(
        status _status, 
        std::string_view details, 
        std::string_view suggestion, 
        const std::source_location& loc = std::source_location::current()
    ) const {
        if (!show_fatal) return;
        std::string msg = std::format("code {:03}: {}\n\t{}\n\t{}", static_cast<int>(_status), status_string(_status), details, suggestion);
        std::cout << std::format("\033[{}m> [{}; {}] [{}] {}\033[0m\n",
            level_color(level::fatal),
            owner,
            get_src_info(loc),
            level_string(level::fatal),
            msg
        );
    }

    template<typename... Args>
    std::string get_input(std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::format(fmt, std::forward<Args>(args)...);
        std::string input;
        std::getline(std::cin, input);
        return input;
    }
private:
    std::string src;

    const std::string_view owner;

    static constexpr std::string_view LOG_FMT = "\033[{}m> [{}; {}] {}\033[0m";

    [[nodiscard]] static std::string get_src_info(const std::source_location& _src) {
        return std::format("{}:{}", std::filesystem::path(_src.file_name()).filename().string(), _src.line());
    }

    [[nodiscard]] static constexpr std::string_view level_string(level _level) {
        switch (_level) {
            case level::debug:      return "DEBUG";
            case level::info:       return "INFO";
            case level::verbose:    return "VERBOSE";
            case level::warn:       return "WARN";
            case level::error:      return "ERROR";
            case level::fatal:      return "FATAL";
        }
        return "UNKNOWN";
    }

    [[nodiscard]] static constexpr std::string_view level_color(level _level) {
        switch (_level) {
            case level::debug:      return "1;34";  // Light Blue
            case level::info:       return "0";     // Default
            case level::verbose:    return "1;34";  // Light Blue
            case level::warn:       return "1;33";  // Yellow
            case level::error:      return "1;31";  // Light Red
            case level::fatal:      return "0;31";  // Red
        }
        return "0";
    }

    [[nodiscard]] static constexpr std::string_view status_string(status _status) {
        switch (_status) {
            case status::null_file      : return "null file";
            case status::file_magic     : return "file magic/signature";
            case status::version        : return "version";
            case status::null_pointer   : return "null pointer";
            case status::type_mismatch  : return "type mismatch";
            case status::bad_value      : return "bad value";
            case status::null_data      : return "null data";
            case status::missing_field  : return "missing field";
            case status::value_mismatch : return "value mismatch";
        }
        return "unknown code";
    }
};

}

#endif