// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_FORECAST_FORECAST_DAY_H_
#define INCLUDE_APIXU_RESPONSE_FORECAST_FORECAST_DAY_H_

#include <string>
#include <vector>

#include "apixu/response/forecast/astro.h"
#include "apixu/response/forecast/day.h"
#include "apixu/response/forecast/hour.h"
#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
namespace forecast {
struct ForecastDay {
    std::string date;
    time_t date_epoch;
    Day day;
    Astro astro;
    std::vector<Hour> hour;

    friend void from_json(const nlohmann::json& j, ForecastDay& f);
};

inline void from_json(const nlohmann::json& j, ForecastDay& f)
{
    f.date = j.at("date");
    f.date_epoch = j.at("date_epoch");
    f.day = j.at("day");
    f.astro = j.at("astro");

    if (j.contains("hour")) {
        f.hour = j.at("hour").get<std::vector<Hour>>();
    }
}
}  // namespace forecast
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_FORECAST_FORECAST_DAY_H_
