// Copyright 2020 <Andrei Avram>
#include <map>
#include <string>

#include "apixu/apixu.h"
#include "apixu/exception/api_exception.h"
#include "gtest/gtest.h"
#include "http_client_mock.cpp"

namespace apixutest {

using apixu::http::Status;

class ApixuSearchTest : public ::testing::Test {
   public:
    const std::string url_ = "http://localhost:5000/search.json";
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

TEST_F(ApixuSearchTest, success)
{
    auto status = Status::Ok;
    std::string body = R"(
           [
               {
                  "id":988,
                  "name":"ABCDEFG",
                  "region":"ABCDEFGHI",
                  "country":"ABCDEFGHIJKLMNOPQRSTUVW",
                  "lat":861.75,
                  "lon":-72.75,
                  "url":"ABCDEFGHIJKLMNOPQ"
               }
           ]
        )";

    auto mock_http_client = HttpClientMock::Create(url_, params_, status, body);

    apixu::Apixu apixu{api_key_, std::move(mock_http_client)};
    const auto& search = apixu.Search(q_);

    EXPECT_EQ(988, *search[0].id);
    EXPECT_EQ("ABCDEFG", search[0].name);
    EXPECT_EQ("ABCDEFGHI", search[0].region);
    EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVW", search[0].country);
    EXPECT_EQ(861.75, search[0].lat);
    EXPECT_EQ(-72.75, search[0].lon);
    EXPECT_EQ("ABCDEFGHIJKLMNOPQ", *search[0].url);
}

TEST_F(ApixuSearchTest, error)
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

    EXPECT_THROW(apixu.Search(q_), apixu::exception::ApiException);
}
}  // namespace apixutest
