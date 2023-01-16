#pragma once

#include <array>
#include <iostream>

namespace Log {
    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
    };

    class LoggerBase {
    protected:
        std::array<std::string_view, 5> levels {{ "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" }};

    public:
        virtual ~LoggerBase() {};
        virtual void write(Level level, std::string_view message) = 0;
    };

    class ConsoleLogger : public LoggerBase {
    public:
        virtual void write(Level level, std::string_view message) override;
    };

    void initGlobal(std::unique_ptr<LoggerBase> logger);
    void write(Level level, std::string_view message);
};
