// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_APIXU_H_
#define INCLUDE_APIXU_APIXU_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "apixu/http/http.h"
#include "apixu/response/condition.h"
#include "apixu/response/current_weather.h"
#include "apixu/response/forecast/weather_forecast.h"
#include "apixu/response/location.h"
#include "apixu/response/weather_history.h"

namespace apixu {
class Apixu {
   public:
    explicit Apixu(const std::string& api_key);

    Apixu(const std::string& api_key, std::unique_ptr<http::Http> http_client);

    std::vector<response::Condition> Conditions() const;

    response::CurrentWeather Current(const std::string& q) const;

    std::vector<response::Location> Search(const std::string& q) const;

    response::forecast::WeatherForecast Forecast(const std::string& q, int days) const;
    response::forecast::WeatherForecast Forecast(const std::string& q, int days, int hour) const;

    response::WeatherHistory History(const std::string& q, const std::string& since) const;
    response::WeatherHistory History(const std::string& q, const std::string& since, const std::string& until) const;

   private:
    const std::string api_key_;
    std::unique_ptr<http::Http> http_client_;

    const std::string api_url_ = "http://localhost:5000/";
    const std::string api_format_ = "json";
    const std::string doc_weather_conditions_url_ = "http://localhost:5000/conditions.json";
    const std::string user_agent_ = "Apixu C++/0.1.0";

    response::forecast::WeatherForecast forecast(http::Parameters& params) const;
    response::WeatherHistory history(http::Parameters& params) const;
    std::string url(const std::string& method) const;
    std::string get(const std::string& url, const http::Parameters& params = {}) const;
};
}  // namespace apixu

#endif  // INCLUDE_APIXU_APIXU_H_
