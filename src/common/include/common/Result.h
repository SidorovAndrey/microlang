#pragma once

#include <string>

class VoidResult {
protected: 
    VoidResult(bool isSuccess, std::string errorMessage)
        : isSuccess(isSuccess), errorMessage(errorMessage)
    {}

public:
    bool isSuccess;
    std::string errorMessage;

    VoidResult(const VoidResult&) = delete;

    VoidResult(VoidResult&& item) 
        : isSuccess(item.isSuccess), errorMessage(item.errorMessage)
    {};

    [[nodiscard]] static VoidResult Ok() { return VoidResult(true, ""); };
    [[nodiscard]] static VoidResult Failure(std::string errorMessage) { return VoidResult(false, errorMessage); };
};

template <typename T = char>
class Result : public VoidResult {
private:
    Result(bool isSuccess, std::string errorMessage, const T& value)
        : VoidResult(isSuccess, errorMessage), data(value)
    {}

    Result(bool isSuccess, std::string errorMessage, T&& value)
        : VoidResult(isSuccess, errorMessage), data(std::move(value))
    {}

public:
    T data;

    Result(const Result<T>&) = delete;
    Result<T>& operator=(const Result<T>&) = delete;

    Result(Result<T>&& item) 
        : VoidResult(item.isSuccess, item.errorMessage), data(item.data)
    {};

    // TODO: move to Result.cpp
    [[nodiscard]] static Result<T> Ok(const T& value) { return Result<T>(true, "", value); }
    [[nodiscard]] static Result<T> Ok(T&& value) { return Result<T>(true, "", std::move(value)); }
    [[nodiscard]] static Result<T> Failure(std::string errorMessage, const T& value) { return Result<T>(false, errorMessage, value); }
    [[nodiscard]] static Result<T> Failure(std::string errorMessage, T&& value) { return Result<T>(false, errorMessage, std::move(value)); }
};

