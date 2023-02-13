// Copyright 2020 <Andrei Avram>
#include <map>
#include <string>
#include <utility>

#include "apixu/http/http.h"
#include "gmock/gmock.h"

namespace apixutest {
using apixu::http::Parameters;
using apixu::http::Status;
using ::testing::Return;
using ::testing::ReturnRef;

class HttpClientMock : public apixu::http::Http {
   public:
    MOCK_METHOD(apixu::http::Response, Get, (const std::string&, (const Parameters&)), (const override));

    static auto Create(const std::string& url, const Parameters& params, Status status, const std::string& body)
    {
        auto response = apixu::http::Response{status, body};
        auto client = std::make_unique<HttpClientMock>();

        EXPECT_CALL(*client, Get(url, params)).Times(1).WillOnce(Return(response));

        return client;
    }
};

}  // namespace apixutest
