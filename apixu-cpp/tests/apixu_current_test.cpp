// Copyright 2020 <Andrei Avram>
#include <map>
#include <string>

#include "apixu/apixu.h"
#include "apixu/exception/api_exception.h"
#include "gtest/gtest.h"
#include "http_client_mock.cpp"

namespace apixutest {

using apixu::http::Status;

class ApixuCurrentTest : public ::testing::Test {
   public:
    const std::string url_ = "http://localhost:5000/current.json";
    const std::string api_key_ = "apikey";
    const std::string q_ = "Paris";
    std::map<std::string, std::string> params_;

   protected:
    void SetUp() override
    {
        params_["key"] = api_key_;
        params_["q"] = q_;
    }
};

TEST_F(ApixuCurrentTest, success)
{
    auto status = Status::Ok;
    std::string body = R"(
           {
              "location": {
                "name": "ABCDEFGHIJKLMNOPQRST",
                "region": "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                "country": "ABCDEFGHIJKLMNOPQRS",
                "lat": 934.25,
                "lon": 617.0,
                "tz_id": "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                "localtime_epoch": 1574708193,
                "localtime": "2019-11-25 18:56:33"
              },
              "current": {
                "last_updated_epoch": 1574708193,
                "last_updated": "2019-11-25 18:56:33",
                "temp_c": 335.5,
                "temp_f": 327.0,
                "is_day": 1,
                "condition": {
                  "text": "ABCDE",
                  "icon": "ABCDEFGHIJKLMNOPQRSTUVW",
                  "code": 623
                },
                "wind_mph": 159.5,
                "wind_kph": -91.0,
                "wind_degree": -6,
                "wind_dir": "ABCDEFGHIJKLMNOPQRSTUVWXYZAB",
                "pressure_mb": -68.25,
                "pressure_in": 2.5,
                "precip_mm": 785.0,
                "precip_in": 588.75,
                "humidity": -11,
                "cloud": 509,
                "feelslike_c": 284.25,
                "feelslike_f": 413.25,
                "vis_km": 356.75,
                "vis_miles": 440.5,
                "uv": -8.0
              }
            }
        )";

    auto mock_http_client = HttpClientMock::Create(url_, params_, status, body);
    apixu::Apixu apixu{api_key_, std::move(mock_http_client)};

    const auto& current = apixu.Current(q_);

    // Location
    EXPECT_EQ("ABCDEFGHIJKLMNOPQRST", current.location.name);
    EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", current.location.region);
    EXPECT_EQ("ABCDEFGHIJKLMNOPQRS", current.location.country);
    EXPECT_EQ(934.25, current.location.lat);
    EXPECT_EQ(617.0, current.location.lon);
    EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", *current.location.timezone);
    EXPECT_EQ(1574708193, *current.location.localtime_epoch);

    struct tm expected {
    };
    expected.tm_sec = 0;
    expected.tm_year = 2019;
    expected.tm_mon = 11 - 1;
    expected.tm_mday = 25;
    expected.tm_hour = 18;
    expected.tm_min = 56;
    auto actual = current.location.localtime;

    EXPECT_EQ(mktime(&expected), mktime(&actual));

    // Current
    EXPECT_EQ(1574708193, *current.current.last_updated_epoch);

    actual = current.current.last_updated;
    EXPECT_EQ(mktime(&expected), mktime(&actual));

    EXPECT_EQ(335.5, *current.current.temp_c);
    EXPECT_EQ(327.0, *current.current.temp_f);
    EXPECT_EQ(true, *current.current.is_day);

    EXPECT_EQ("ABCDE", *current.current.condition.text);
    EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVW", *current.current.condition.icon);
    EXPECT_EQ(623, *current.current.condition.code);

    EXPECT_EQ(159.5, *current.current.wind_mph);
    EXPECT_EQ(-91.0, *current.current.wind_kph);
    EXPECT_EQ(-6, *current.current.wind_degree);
    EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVWXYZAB", *current.current.wind_dir);
    EXPECT_EQ(-68.25, *current.current.pressure_mb);
    EXPECT_EQ(2.5, *current.current.pressure_in);
    EXPECT_EQ(785.0, *current.current.precip_mm);
    EXPECT_EQ(588.75, *current.current.precip_in);
    EXPECT_EQ(-11, *current.current.humidity);
    EXPECT_EQ(509, *current.current.cloud);
    EXPECT_EQ(284.25, *current.current.feelslike_c);
    EXPECT_EQ(413.25, *current.current.feelslike_f);
    EXPECT_EQ(356.75, *current.current.vis_km);
    EXPECT_EQ(440.5, *current.current.vis_miles);
    EXPECT_EQ(-8.0, *current.current.uv);
}

TEST_F(ApixuCurrentTest, error)
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

    EXPECT_THROW(apixu.Current(q_), apixu::exception::ApiException);
}
}  // namespace apixutest
