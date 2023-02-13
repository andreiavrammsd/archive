// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_LOCATION_H_
#define INCLUDE_APIXU_RESPONSE_LOCATION_H_

#include <memory>
#include <string>

#include "apixu/time.h"
#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
struct Location {
    std::shared_ptr<int> id;
    std::string name;
    std::string region;
    std::string country;
    double lat{};
    double lon{};
    std::shared_ptr<std::string> url{};
    std::shared_ptr<std::string> timezone{};
    std::shared_ptr<int64_t> localtime_epoch{};
    struct tm localtime {
    };

    friend void from_json(const nlohmann::json& j, Location& l);
};

inline void from_json(const nlohmann::json& j, Location& l)
{
    if (j.contains("id")) {
        l.id = std::make_shared<int>(j.at("id"));
    }

    l.name = j.at("name");
    l.region = j.at("region");
    l.country = j.at("country");
    l.lat = j.at("lat");
    l.lon = j.at("lon");

    if (j.contains("url")) {
        l.url = std::make_shared<std::string>(j.at("url"));
    }

    if (j.contains("tz_id")) {
        l.timezone = std::make_shared<std::string>(j.at("tz_id"));
    }

    if (j.contains("localtime_epoch")) {
        l.localtime_epoch = std::make_shared<int64_t>(j.at("localtime_epoch"));
    }

    if (j.contains("localtime")) {
        l.localtime = Time::Parse(j.at("localtime"));
    }
}
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_LOCATION_H_
