// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_FORECAST_HOUR_H_
#define INCLUDE_APIXU_RESPONSE_FORECAST_HOUR_H_

#include <cstdint>
#include <string>

#include "apixu/response/current_condition.h"
#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
namespace forecast {
struct Hour {
    time_t time_epoch{};
    std::string time;
    double temp_c;
    double temp_f;
    bool is_day;
    CurrentCondition condition;
    double wind_mph;
    double wind_kph;
    std::int64_t wind_degree;
    std::string wind_dir;
    std::int64_t pressure_mb;
    double pressure_in;
    double precip_mm;
    double precip_in;
    std::int64_t humidity;
    std::int64_t cloud;
    double feels_like_c;
    double feels_like_f;
    double wind_chill_c;
    double wind_chill_f;
    double heat_index_c;
    double heat_index_f;
    double dew_point_c;
    double dew_point_f;
    bool will_rain;
    std::string chance_of_rain;
    bool will_snow;
    std::string chance_of_snow;
    std::int64_t vis_km;
    std::int64_t vis_miles;
    double gust_mph;
    double gust_kph;

    friend void from_json(const nlohmann::json& j, Hour& h);
};

inline void from_json(const nlohmann::json& j, Hour& h)
{
    if (j.contains("time_epoch")) {
        h.time_epoch = j.at("time_epoch");
    }

    if (j.contains("time")) {
        h.time = j.at("time");
    }

    if (j.contains("temp_c")) {
        h.temp_c = j.at("temp_c");
    }

    if (j.contains("temp_f")) {
        h.temp_f = j.at("temp_f");
    }

    if (j.contains("is_day")) {
        h.is_day = j.at("is_day").get<int>();
    }

    if (j.contains("condition")) {
        h.condition = j.at("condition");
    }

    if (j.contains("wind_mph")) {
        h.wind_mph = j.at("wind_mph");
    }

    if (j.contains("wind_kph")) {
        h.wind_kph = j.at("wind_kph");
    }

    if (j.contains("wind_degree")) {
        h.wind_degree = j.at("wind_degree");
    }

    if (j.contains("wind_dir")) {
        h.wind_dir = j.at("wind_dir");
    }

    if (j.contains("pressure_mb")) {
        h.pressure_mb = j.at("pressure_mb");
    }

    if (j.contains("pressure_in")) {
        h.pressure_in = j.at("pressure_in");
    }

    if (j.contains("precip_mm")) {
        h.precip_mm = j.at("precip_mm");
    }

    if (j.contains("precip_in")) {
        h.precip_in = j.at("precip_in");
    }

    if (j.contains("humidity")) {
        h.humidity = j.at("humidity");
    }

    if (j.contains("cloud")) {
        h.cloud = j.at("cloud");
    }

    if (j.contains("feelslike_c")) {
        h.feels_like_c = j.at("feelslike_c");
    }

    if (j.contains("feelslike_f")) {
        h.feels_like_f = j.at("feelslike_f");
    }

    if (j.contains("windchill_c")) {
        h.wind_chill_c = j.at("windchill_c");
    }

    if (j.contains("windchill_f")) {
        h.wind_chill_f = j.at("windchill_f");
    }

    if (j.contains("heatindex_c")) {
        h.heat_index_c = j.at("heatindex_c");
    }

    if (j.contains("heatindex_f")) {
        h.heat_index_f = j.at("heatindex_f");
    }

    if (j.contains("dewpoint_c")) {
        h.dew_point_c = j.at("dewpoint_c");
    }

    if (j.contains("dewpoint_f")) {
        h.dew_point_f = j.at("dewpoint_f");
    }

    if (j.contains("will_it_rain")) {
        h.will_rain = j.at("will_it_rain").get<int>();
    }

    if (j.contains("chance_of_rain")) {
        h.chance_of_rain = j.at("chance_of_rain");
    }

    if (j.contains("will_it_snow")) {
        h.will_snow = j.at("will_it_snow").get<int>();
    }

    if (j.contains("chance_of_snow")) {
        h.chance_of_snow = j.at("chance_of_snow");
    }

    if (j.contains("vis_km")) {
        h.vis_km = j.at("vis_km");
    }

    if (j.contains("vis_miles")) {
        h.vis_miles = j.at("vis_miles");
    }

    if (j.contains("gust_mph")) {
        h.gust_mph = j.at("gust_mph");
    }

    if (j.contains("gust_kph")) {
        h.gust_kph = j.at("gust_kph");
    }
}
}  // namespace forecast
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_FORECAST_HOUR_H_
