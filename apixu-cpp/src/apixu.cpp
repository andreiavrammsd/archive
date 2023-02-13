// Copyright 2020 <Andrei Avram>
#include "apixu/apixu.h"

#include <string>
#include <utility>
#include <vector>

#include "apixu/exception/api_exception.h"
#include "apixu/http/client.h"
#include "apixu/response/error.h"

namespace apixu {
Apixu::Apixu(const std::string& api_key) : api_key_(api_key)
{
    http_client_ = std::make_unique<http::Client>(user_agent_);
}

Apixu::Apixu(const std::string& api_key, std::unique_ptr<http::Http> http_client)
    : api_key_(api_key), http_client_(std::move(http_client))
{
    if (http_client_ == nullptr) {
        throw std::invalid_argument{"http client is null"};
    }
}

std::vector<response::Condition> Apixu::Conditions() const
{
    try {
        return nlohmann::json::parse(get(doc_weather_conditions_url_));
    }
    catch (exception::ApiException& e) {
        throw exception::ApiException(e.what(), e.getCode());
    }
    catch (std::exception& e) {
        throw exception::ApixuException(e.what());
    }
}

response::CurrentWeather Apixu::Current(const std::string& q) const
{
    http::Parameters params;
    params["key"] = api_key_;
    params["q"] = q;

    try {
        return nlohmann::json::parse(get(url("current"), params));
    }
    catch (exception::ApiException& e) {
        throw exception::ApiException(e.what(), e.getCode());
    }
    catch (std::exception& e) {
        throw exception::ApixuException(e.what());
    }
}

std::vector<response::Location> Apixu::Search(const std::string& q) const
{
    http::Parameters params;
    params["key"] = api_key_;
    params["q"] = q;

    try {
        return nlohmann::json::parse(get(url("search"), params));
    }
    catch (exception::ApiException& e) {
        throw exception::ApiException(e.what(), e.getCode());
    }
    catch (std::exception& e) {
        throw exception::ApixuException(e.what());
    }
}

response::forecast::WeatherForecast Apixu::Forecast(const std::string& q, const int days) const
{
    http::Parameters params;
    params["key"] = api_key_;
    params["q"] = q;
    params["days"] = std::to_string(days);

    return forecast(params);
}

response::forecast::WeatherForecast Apixu::Forecast(const std::string& q, const int days, const int hour) const
{
    http::Parameters params;
    params["key"] = api_key_;
    params["q"] = q;
    params["days"] = std::to_string(days);
    params["hour"] = std::to_string(hour);

    return forecast(params);
}

response::WeatherHistory Apixu::History(const std::string& q, const std::string& since) const
{
    http::Parameters params;
    params["key"] = api_key_;
    params["q"] = q;
    params["dt"] = since;

    return history(params);
}

response::WeatherHistory Apixu::History(const std::string& q, const std::string& since, const std::string& until) const
{
    http::Parameters params;
    params["key"] = api_key_;
    params["q"] = q;
    params["dt"] = since;
    params["end_dt"] = until;

    return history(params);
}

response::forecast::WeatherForecast Apixu::forecast(http::Parameters& params) const
{
    try {
        return nlohmann::json::parse(get(url("forecast"), params));
    }
    catch (exception::ApiException& e) {
        throw exception::ApiException(e.what(), e.getCode());
    }
    catch (std::exception& e) {
        throw exception::ApixuException(e.what());
    }
}

response::WeatherHistory Apixu::history(http::Parameters& params) const
{
    try {
        return nlohmann::json::parse(get(url("history"), params));
    }
    catch (exception::ApiException& e) {
        throw exception::ApiException(e.what(), e.getCode());
    }
    catch (std::exception& e) {
        throw exception::ApixuException(e.what());
    }
}

std::string Apixu::url(const std::string& method) const { return api_url_ + method + "." + api_format_; }

std::string Apixu::get(const std::string& url, const http::Parameters& params) const
{
    auto response = http_client_->Get(url, params);

    if (response.status >= http::Status::InternalServerError) {
        throw exception::ApixuException("Internal Server Error");
    }
    else if (response.status >= http::Status::BadRequest) {
        response::ErrorResponse errRes = nlohmann::json::parse(response.body);
        throw exception::ApiException(errRes.error.message, errRes.error.code);
    }

    return response.body;
}
}  // namespace apixu
