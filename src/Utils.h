#pragma once

#include <vector>

namespace Utils {
    [[nodiscard]] std::vector<uint8_t> uint32ToByteArray(uint32_t value);
    [[nodiscard]] std::vector<uint8_t> intToByteArray(int value);
    [[nodiscard]] uint32_t byteArrayToUint32(const std::vector<uint8_t>& value);
}
