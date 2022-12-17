#pragma once

#include <vector>

namespace Utils {
    std::vector<uint8_t> uint32ToByteArray(uint32_t value);
    std::vector<uint8_t> intToByteArray(int value);
    uint32_t byteArrayToUint32(const std::vector<uint8_t>& value);
}
