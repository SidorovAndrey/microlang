#pragma once

#include <string>

template <typename T>
struct Result {
    bool isSuccess;
    std::string errorMessage;
    T data;

    Result(bool isSuccess, std::string errorMessage, T data)
        : isSuccess(isSuccess), errorMessage(errorMessage), data(std::move(data))
    {}

    Result(const Result<T>&) = delete;

    Result(Result<T>&& item) 
        : isSuccess(item.isSuccess), errorMessage(item.errorMessage), data(item.data)
    {};
};

