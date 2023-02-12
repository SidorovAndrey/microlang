#include "common/Log.h"

namespace Log {
    static std::unique_ptr<LoggerBase> globalLogger;

    void initGlobal(std::unique_ptr<LoggerBase> logger) {
        globalLogger = std::move(logger);
    };

    void write(Level level, std::string_view message) {
        globalLogger->write(level, message);
    }

    void ConsoleLogger::write(Level level, std::string_view message) {
        std::cout << "[" << levels[level] << "]: " << message << "\n";
    }
};
