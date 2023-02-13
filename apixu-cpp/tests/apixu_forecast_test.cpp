// Copyright 2020 <Andrei Avram>
#include <map>
#include <string>

#include "apixu/apixu.h"
#include "apixu/exception/api_exception.h"
#include "gtest/gtest.h"
#include "http_client_mock.cpp"

namespace apixutest {

using apixu::http::Status;

class ApixuForecastTest : public ::testing::Test {
   public:
    const std::string url_ = "http://localhost:5000/forecast.json";
    const std::string api_key_ = "apikey";
    const std::string q_ = "Paris";
    std::map<std::string, std::string> params_;

   protected:
    void SetUp() override
    {
        params_["key"] = api_key_;
        params_["q"] = q_;
        params_["days"] = "1";
        params_["hour"] = "3";
    }
};

TEST_F(ApixuForecastTest, success)
{
    Status status = Status::Ok;
    std::string body = R"(
           {
               "location":{
                  "name":"ABCDEFGHIJKLMNOPQRST",
                  "region":"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                  "country":"ABCDEFGHIJKLMNOPQRS",
                  "lat":934.25,
                  "lon":617.0,
                  "tz_id":"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                  "localtime_epoch":1574708193,
                  "localtime":"2019-11-25 18:56:33"
               },
               "current":{
                  "last_updated_epoch":1574708193,
                  "last_updated":"2019-11-25 18:56:33",
                  "temp_c":335.5,
                  "temp_f":327.0,
                  "is_day":400,
                  "condition":{
                     "text":"ABCDE",
                     "icon":"ABCDEFGHIJKLMNOPQRSTUVW",
                     "code":623
                  },
                  "wind_mph":159.5,
                  "wind_kph":-91.0,
                  "wind_degree":-6,
                  "wind_dir":"ABCDEFGHIJKLMNOPQRSTUVWXYZAB",
                  "pressure_mb":-68.25,
                  "pressure_in":2.5,
                  "precip_mm":785.0,
                  "precip_in":588.75,
                  "humidity":-11,
                  "cloud":509,
                  "feelslike_c":284.25,
                  "feelslike_f":413.25,
                  "vis_km":356.75,
                  "vis_miles":440.5,
                  "uv":-8.0
               },
               "forecast":{
                  "forecastday":[
                     {
                        "date":"ABCDEFGHI",
                        "date_epoch":1574708193,
                        "day":{
                           "maxtemp_c":570.75,
                           "maxtemp_f":920.0,
                           "mintemp_c":484.0,
                           "mintemp_f":544.0,
                           "avgtemp_c":518.25,
                           "avgtemp_f":645.5,
                           "maxwind_mph":244.25,
                           "maxwind_kph":497.0,
                           "totalprecip_mm":582.75,
                           "totalprecip_in":298.0,
                           "avgvis_km":377.0,
                           "avgvis_miles":576.25,
                           "avghumidity":406.5,
                           "condition":{
                              "text":"ABCDEFGHIJKLM",
                              "icon":"ABCDEFGHIJKLMNO",
                              "code":430
                           },
                           "uv":978.25
                        },
                        "astro":{
                           "sunrise":"ABCDEFGHIJKLMNOPQRSTUVWX",
                           "sunset":"ABCDEFGHIJKLMNOPQRST",
                           "moonrise":"ABCDEFGHIJKLMNOPQRSTUVWXYZAB",
                           "moonset":"ABCDEFG"
                        },
                        "hour":[
                           {
                              "time_epoch":1574708193,
                              "time":"2019-11-25 18:56:33",
                              "temp_c":519.25,
                              "temp_f":282.75,
                              "is_day":147,
                              "condition":{
                                 "text":"ABCDEFGHIJKLMNOPQRSTU",
                                 "icon":"ABCDEFGHIJKLMN",
                                 "code":861
                              },
                              "wind_mph":499.0,
                              "wind_kph":860.25,
                              "wind_degree":818,
                              "wind_dir":"ABCDEFGHIJKLMNOPQRSTUV",
                              "pressure_mb":360.75,
                              "pressure_in":793.5,
                              "precip_mm":373.5,
                              "precip_in":474.75,
                              "humidity":397,
                              "cloud":164,
                              "feelslike_c":922.25,
                              "feelslike_f":426.75,
                              "vis_km":861.0,
                              "vis_miles":723.0,
                              "gust_mph":688.0,
                              "gust_kph":286.75,
                              "uv":377.25
                           }
                        ]
                     }
                  ]
               }
            }
        )";

    auto mock_http_client = HttpClientMock::Create(url_, params_, status, body);
    apixu::Apixu apixu{api_key_, std::move(mock_http_client)};

    const auto& forecast = apixu.Forecast(q_, 1, 3);

    EXPECT_EQ("ABCDEFGHIJKLMNOPQRST", forecast.location.name);
}

TEST_F(ApixuForecastTest, error)
{
    auto status = Status::BadRequest;
    std::string body = R"(
            {
                "error": {
                  "message":"err",
                  "code":1
               }
            }
        )";

    auto mock_http_client = HttpClientMock::Create(url_, params_, status, body);
    apixu::Apixu apixu{api_key_, std::move(mock_http_client)};

    EXPECT_THROW(apixu.Forecast(q_, 1, 3), apixu::exception::ApiException);
}
}  // namespace apixutest
