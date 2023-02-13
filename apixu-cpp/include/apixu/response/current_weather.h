// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_CURRENT_WEATHER_H_
#define INCLUDE_APIXU_RESPONSE_CURRENT_WEATHER_H_

#include "apixu/response/current.h"
#include "apixu/response/location.h"
#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
struct CurrentWeather {
    Location location;
    Current current;

    friend void from_json(const nlohmann::json& j, CurrentWeather& cw);
};

inline void from_json(const nlohmann::json& j, CurrentWeather& cw)
{
    cw.location = j.at("location");
    cw.current = j.at("current");
}
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_CURRENT_WEATHER_H_
