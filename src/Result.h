#pragma once

template <typename T>
struct Result {
    bool isSuccess;
    std::string errorMessage;
    T data;
};

