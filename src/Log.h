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

    void initGlobal(std::ostream* stream);
    void write(Level level, std::string_view message);

    class Logger {
    private:
        std::ostream* m_output; // TODO: use inheritance with specific Logger for cout
        std::array<std::string_view, 5> m_levels {{ "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" }};

    public:
        Logger(std::ostream* stream) : m_output(stream) {};
        inline void write(Level level, std::string_view message) { 
            (*m_output) << "[" << m_levels[level] << "]: " << message << "\n"; 
        };
    };
};
