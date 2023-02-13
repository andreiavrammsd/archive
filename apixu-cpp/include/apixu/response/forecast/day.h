// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_FORECAST_DAY_H_
#define INCLUDE_APIXU_RESPONSE_FORECAST_DAY_H_

#include "apixu/response/current_condition.h"
#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
namespace forecast {
struct Day {
    float max_temp_celsius{};
    float max_temp_fahrenheit{};
    float min_temp_celsius{};
    float min_temp_fahrenheit{};
    float avg_temp_celsius{};
    float avg_temp_fahrenheit{};
    float max_wind_mph{};
    float max_wind_kph{};
    float total_precip_mm{};
    float total_precip_in{};
    float avg_vis_km{};
    float avg_vis_miles{};
    float avg_humidity{};
    CurrentCondition condition;
    float uv{};

    friend void from_json(const nlohmann::json& j, Day& d);
};

inline void from_json(const nlohmann::json& j, Day& d)
{
    if (j.contains("maxtemp_c")) {
        d.max_temp_celsius = j.at("maxtemp_c");
    }

    if (j.contains("maxtemp_f")) {
        d.max_temp_fahrenheit = j.at("maxtemp_f");
    }

    if (j.contains("mintemp_c")) {
        d.min_temp_celsius = j.at("mintemp_c");
    }

    if (j.contains("mintemp_f")) {
        d.min_temp_fahrenheit = j.at("mintemp_f");
    }

    if (j.contains("avgtemp_c")) {
        d.avg_temp_celsius = j.at("avgtemp_c");
    }

    if (j.contains("avgtemp_f")) {
        d.avg_temp_fahrenheit = j.at("avgtemp_f");
    }

    if (j.contains("maxwind_mph")) {
        d.max_wind_mph = j.at("maxwind_mph");
    }

    if (j.contains("maxwind_kph")) {
        d.max_wind_kph = j.at("maxwind_kph");
    }

    if (j.contains("totalprecip_mm")) {
        d.total_precip_mm = j.at("totalprecip_mm");
    }

    if (j.contains("totalprecip_in")) {
        d.total_precip_in = j.at("totalprecip_in");
    }

    if (j.contains("avgvis_km")) {
        d.avg_vis_km = j.at("avgvis_km");
    }

    if (j.contains("avgvis_miles")) {
        d.avg_vis_miles = j.at("avgvis_miles");
    }

    if (j.contains("avghumidity")) {
        d.avg_humidity = j.at("avghumidity");
    }

    if (j.contains("condition")) {
        d.condition = j.at("condition");
    }

    if (j.contains("uv")) {
        d.uv = j.at("uv");
    }
}
}  // namespace forecast
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_FORECAST_DAY_H_
