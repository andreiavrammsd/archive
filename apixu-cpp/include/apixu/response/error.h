// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_ERROR_H_
#define INCLUDE_APIXU_RESPONSE_ERROR_H_

#include <string>

#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
struct Error {
    int code{};
    std::string message;

    friend void from_json(const nlohmann::json& j, Error& e);
};

inline void from_json(const nlohmann::json& j, Error& e)
{
    e.code = j.at("code");
    e.message = j.at("message");
}

struct ErrorResponse {
    Error error;

    friend void from_json(const nlohmann::json& j, ErrorResponse& e);
};

inline void from_json(const nlohmann::json& j, ErrorResponse& e) { e.error = j.at("error"); }
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_ERROR_H_
