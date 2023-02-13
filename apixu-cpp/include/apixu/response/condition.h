// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_CONDITION_H_
#define INCLUDE_APIXU_RESPONSE_CONDITION_H_

#include <string>

#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
struct Condition {
    int code{};
    std::string day{};
    std::string night{};
    int icon{};

    friend void from_json(const nlohmann::json& j, Condition& c);
};

inline void from_json(const nlohmann::json& j, Condition& c)
{
    c.code = j.at("code");
    c.day = j.at("day");
    c.night = j.at("night");
    c.icon = j.at("icon");
}
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_CONDITION_H_
