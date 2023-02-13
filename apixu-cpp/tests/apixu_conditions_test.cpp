// Copyright 2020 <Andrei Avram>
#include <map>
#include <string>

#include "apixu/apixu.h"
#include "apixu/exception/api_exception.h"
#include "gtest/gtest.h"
#include "http_client_mock.cpp"

namespace apixutest {

using apixu::http::Status;

class ApixuConditionsTest : public ::testing::Test {
   public:
    const std::string url_ = "http://localhost:5000/conditions.json";
};

TEST_F(ApixuConditionsTest, success)
{
    auto status = Status::Ok;
    std::string body = R"(
            [
               {
                  "code":1,
                  "day":"day",
                  "night":"night",
                  "icon":22
               }
            ]
        )";

    auto mock_http_client = HttpClientMock::Create(url_, std::map<std::string, std::string>{}, status, body);
    apixu::Apixu apixu{"", std::move(mock_http_client)};

    const auto& conditions = apixu.Conditions();

    EXPECT_EQ(1, conditions.size());
    EXPECT_EQ(1, conditions[0].code);
    EXPECT_EQ("day", conditions[0].day);
    EXPECT_EQ("night", conditions[0].night);
    EXPECT_EQ(22, conditions[0].icon);
}

TEST_F(ApixuConditionsTest, error)
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

    auto mock_http_client = HttpClientMock::Create(url_, std::map<std::string, std::string>{}, status, body);
    apixu::Apixu apixu{"", std::move(mock_http_client)};

    EXPECT_THROW(apixu.Conditions(), apixu::exception::ApixuException);
}
}  // namespace apixutest
