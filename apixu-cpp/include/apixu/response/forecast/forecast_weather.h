// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_FORECAST_FORECAST_WEATHER_H_
#define INCLUDE_APIXU_RESPONSE_FORECAST_FORECAST_WEATHER_H_

#include <vector>

#include "apixu/response/forecast/forecast_day.h"
#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
namespace forecast {
struct ForecastWeather {
    std::vector<ForecastDay> forecast_day;

    friend void from_json(const nlohmann::json& j, ForecastWeather& f);
};

inline void from_json(const nlohmann::json& j, ForecastWeather& f)
{
    f.forecast_day = j.at("forecastday").get<std::vector<ForecastDay>>();
}
}  // namespace forecast
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_FORECAST_FORECAST_WEATHER_H_
