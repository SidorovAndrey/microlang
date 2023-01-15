#include "Log.h"

namespace Log {
    static std::unique_ptr<Logger> logger;

    void initGlobal(std::ostream* stream) {
        logger = std::make_unique<Logger>(stream);
    };

    void write(Level level, std::string_view message) {
        logger->write(level, message);
    }
};
