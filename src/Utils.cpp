#include "Utils.h"

std::vector<uint8_t> Utils::uint32ToByteArray(uint32_t value) {
    std::vector<uint8_t> res = { 
        static_cast<uint8_t>((value >> 24) & 0xFF),
        static_cast<uint8_t>((value >> 16) & 0xFF),
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>((value) & 0xFF) 
    };

    return res;
}


std::vector<uint8_t> Utils::intToByteArray(int value) {
    std::vector<uint8_t> res = { 
        static_cast<uint8_t>((value >> 24) & 0xFF),
        static_cast<uint8_t>((value >> 16) & 0xFF),
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>((value) & 0xFF) 
    };

    return res;
}

uint32_t Utils::byteArrayToUint32(const std::vector<uint8_t>& value) {
    uint32_t res = 0;
    res |= value[0] << 24;
    res |= value[1] << 16;
    res |= value[2] << 8;
    res |= value[3];
    return res;
}

