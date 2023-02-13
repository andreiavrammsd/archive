// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_FORECAST_ASTRO_H_
#define INCLUDE_APIXU_RESPONSE_FORECAST_ASTRO_H_

#include <string>

#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
namespace forecast {
struct Astro {
    std::string sunrise;
    std::string sunset;
    std::string moonrise;
    std::string moonset;
    std::string moon_phase;
    std::string moon_illumination;

    friend void from_json(const nlohmann::json& j, Astro& d);
};

inline void from_json(const nlohmann::json& j, Astro& d)
{
    if (j.contains("sunrise")) {
        d.sunrise = j.at("sunrise");
    }

    if (j.contains("sunset")) {
        d.sunset = j.at("sunset");
    }

    if (j.contains("moonrise")) {
        d.moonrise = j.at("moonrise");
    }

    if (j.contains("moonset")) {
        d.moonset = j.at("moonset");
    }

    if (j.contains("moon_phase")) {
        d.moon_phase = j.at("moon_phase");
    }

    if (j.contains("moon_illumination")) {
        d.moon_illumination = j.at("moon_illumination");
    }
}
}  // namespace forecast
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_FORECAST_ASTRO_H_
