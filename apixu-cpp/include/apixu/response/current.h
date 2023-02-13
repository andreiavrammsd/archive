// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_RESPONSE_CURRENT_H_
#define INCLUDE_APIXU_RESPONSE_CURRENT_H_

#include <ctime>
#include <memory>
#include <string>

#include "./current_condition.h"
#include "apixu/time.h"
#include "nlohmann/json.hpp"

namespace apixu {
namespace response {
struct Current {
    std::shared_ptr<int> last_updated_epoch{};
    struct tm last_updated {
    };
    std::shared_ptr<float> temp_c{};
    std::shared_ptr<float> temp_f{};
    std::shared_ptr<bool> is_day{};
    CurrentCondition condition;
    std::shared_ptr<float> wind_mph{};
    std::shared_ptr<float> wind_kph{};
    std::shared_ptr<int> wind_degree{};
    std::shared_ptr<std::string> wind_dir{};
    std::shared_ptr<float> pressure_mb{};
    std::shared_ptr<float> pressure_in{};
    std::shared_ptr<float> precip_mm{};
    std::shared_ptr<float> precip_in{};
    std::shared_ptr<int> humidity{};
    std::shared_ptr<int> cloud{};
    std::shared_ptr<float> feelslike_c{};
    std::shared_ptr<float> feelslike_f{};
    std::shared_ptr<float> vis_km{};
    std::shared_ptr<float> vis_miles{};
    std::shared_ptr<float> uv{};
    std::shared_ptr<float> gust_mph{};
    std::shared_ptr<float> gust_kph{};

    friend void from_json(const nlohmann::json& j, Current& c);
};

inline void from_json(const nlohmann::json& j, Current& c)
{
    if (j.contains("last_updated_epoch")) {
        c.last_updated_epoch = std::make_shared<int>(j.at("last_updated_epoch"));
    }

    if (j.contains("last_updated")) {
        c.last_updated = Time::Parse(j.at("last_updated"));
    }

    if (j.contains("temp_c")) {
        c.temp_c = std::make_shared<float>(j.at("temp_c"));
    }

    if (j.contains("temp_f")) {
        c.temp_f = std::make_shared<float>(j.at("temp_f"));
    }

    if (j.contains("is_day")) {
        c.is_day = std::make_shared<bool>(j.at("is_day").get<int>());
    }

    c.condition = j.at("condition");

    if (j.contains("wind_mph")) {
        c.wind_mph = std::make_shared<float>(j.at("wind_mph"));
    }

    if (j.contains("wind_kph")) {
        c.wind_kph = std::make_shared<float>(j.at("wind_kph"));
    }

    if (j.contains("wind_degree")) {
        c.wind_degree = std::make_shared<int>(j.at("wind_degree"));
    }

    if (j.contains("wind_dir")) {
        c.wind_dir = std::make_shared<std::string>(j.at("wind_dir"));
    }

    if (j.contains("pressure_mb")) {
        c.pressure_mb = std::make_shared<float>(j.at("pressure_mb"));
    }

    if (j.contains("pressure_in")) {
        c.pressure_in = std::make_shared<float>(j.at("pressure_in"));
    }

    if (j.contains("precip_mm")) {
        c.precip_mm = std::make_shared<float>(j.at("precip_mm"));
    }

    if (j.contains("precip_in")) {
        c.precip_in = std::make_shared<float>(j.at("precip_in"));
    }

    if (j.contains("humidity")) {
        c.humidity = std::make_shared<int>(j.at("humidity"));
    }

    if (j.contains("cloud")) {
        c.cloud = std::make_shared<int>(j.at("cloud"));
    }

    if (j.contains("feelslike_c")) {
        c.feelslike_c = std::make_shared<float>(j.at("feelslike_c"));
    }

    if (j.contains("feelslike_f")) {
        c.feelslike_f = std::make_shared<float>(j.at("feelslike_f"));
    }

    if (j.contains("vis_km")) {
        c.vis_km = std::make_shared<float>(j.at("vis_km"));
    }

    if (j.contains("vis_miles")) {
        c.vis_miles = std::make_shared<float>(j.at("vis_miles"));
    }

    if (j.contains("uv")) {
        c.uv = std::make_shared<float>(j.at("uv"));
    }

    if (j.contains("gust_mph")) {
        c.gust_mph = std::make_shared<float>(j.at("gust_mph"));
    }

    if (j.contains("gust_kph")) {
        c.gust_kph = std::make_shared<float>(j.at("gust_kph"));
    }
}
}  // namespace response
}  // namespace apixu

#endif  // INCLUDE_APIXU_RESPONSE_CURRENT_H_
