// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_WEATHER_HISTORY_H_
#define INCLUDE_APIXU_RESPONSE_WEATHER_HISTORY_H_

#include "apixu/response/forecast/forecast_weather.h"
#include "apixu/response/location.h"
#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
struct WeatherHistory {
    Location location;
    forecast::ForecastWeather forecast;

    friend void from_json(const nlohmann::json& j, WeatherHistory& h);
};

inline void from_json(const nlohmann::json& j, WeatherHistory& h)
{
    h.location = j.at("location");
    h.forecast = j.at("forecast");
}
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_WEATHER_HISTORY_H_
