// Copyright 2020 <Andrei Avram>
#include <map>
#include <string>

#include "apixu/apixu.h"
#include "apixu/exception/api_exception.h"
#include "gtest/gtest.h"
#include "http_client_mock.cpp"

namespace apixutest {

using apixu::http::Status;

class ApixuHistoryTest : public ::testing::Test {
   public:
    const std::string url_ = "http://localhost:5000/history.json";
    const std::string api_key_ = "apikey";
    const std::string q_ = "Paris";
    std::map<std::string, std::string> params_;

   protected:
    void SetUp() override
    {
        params_["key"] = api_key_;
        params_["q"] = q_;
        params_["dt"] = "2019-01-01";
    }
};

TEST_F(ApixuHistoryTest, success)
{
    auto status = Status::Ok;
    std::string body = R"(
           {
              "location": {
                "name": "ABCDEFGHIJK",
                "region": "ABCDEFGHIJKLMNOP",
                "country": "ABCDEFGHIJKL",
                "lat": 504.0,
                "lon": 70.0,
                "tz_id": "ABCD",
                "localtime_epoch": -24,
                "localtime": "ABCDEFG"
              },
              "forecast": {
                "forecastday": [
                  {
                    "date": "ABCDEFGHIJKLMNOPQRSTUVWXYZABC",
                    "date_epoch": 319,
                    "day": {
                      "maxtemp_c": 66.5,
                      "maxtemp_f": 324.75,
                      "mintemp_c": 320.25,
                      "mintemp_f": 178.25,
                      "avgtemp_c": 50.5,
                      "avgtemp_f": -30.5,
                      "maxwind_mph": 29.0,
                      "maxwind_kph": 45.0,
                      "totalprecip_mm": 711.25,
                      "totalprecip_in": 300.0,
                      "avgvis_km": 84.25,
                      "avgvis_miles": 768.0,
                      "avghumidity": 775.0,
                      "condition": {
                        "text": "ABCDEFGHIJKLMNOPQRSTUVWX",
                        "icon": "ABCDEFG",
                        "code": 174
                      },
                      "uv": 128.5
                    },
                    "astro": {
                      "sunrise": "ABCDEFGHIJKLMNOPQRSTU",
                      "sunset": "ABCDEFGHIJKL",
                      "moonrise": "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                      "moonset": "ABCDEFGHIJKLMN",
                      "moon_phase": "ABCDEFGHIJKLMNOPQRSTU",
                      "moon_illumination": "ABCDEFGHIJKL"
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

    const auto& history = apixu.History(q_, "2019-01-01");

    EXPECT_EQ("ABCDEFGHIJK", history.location.name);
}

TEST_F(ApixuHistoryTest, error)
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

    EXPECT_THROW(apixu.History(q_, "2019-01-01"), apixu::exception::ApiException);
}
}  // namespace apixutest
