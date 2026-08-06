#pragma once

#include <memory>
#include <string>
#include <format>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

class logger
{
public:
    enum class level { trace, info, warn, error, critical };

    static void init();
    static void shutdown();

    template<typename... Args>
    static void log(level lvl, std::format_string<Args...> fmt, Args&&... args)
    {
        const auto now = std::chrono::system_clock::now();
        const auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm_buf{};
#ifdef _WIN32
        localtime_s(&tm_buf, &time);
#else
        localtime_r(&time, &tm_buf);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm_buf, "%H:%M:%S");

        const char* level_str = "INFO";
        switch (lvl)
        {
            case level::trace:    level_str = "TRACE"; break;
            case level::info:     level_str = "INFO";  break;
            case level::warn:     level_str = "WARN";  break;
            case level::error:    level_str = "ERROR"; break;
            case level::critical: level_str = "CRIT";  break;
        }

        const std::string message = std::format(fmt, std::forward<Args>(args)...);
        std::ostream& out = (lvl >= level::error) ? std::cerr : std::cout;
        out << "[" << oss.str() << "][" << level_str << "] " << message << '\n';
    }

private:
    static inline bool s_initialized = false;
};

#define LOG_TRACE(...)    ::logger::log(::logger::level::trace, __VA_ARGS__)
#define LOG_INFO(...)     ::logger::log(::logger::level::info, __VA_ARGS__)
#define LOG_WARN(...)     ::logger::log(::logger::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)    ::logger::log(::logger::level::error, __VA_ARGS__)
#define LOG_CRITICAL(...) ::logger::log(::logger::level::critical, __VA_ARGS__)
