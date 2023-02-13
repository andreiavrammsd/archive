// Copyright 2020 <Andrei Avram>
#include "apixu/apixu.h"

#include <exception>
#include <memory>

#include "gtest/gtest.h"

namespace apixutest {

class ApixuTest : public ::testing::Test {
};

TEST_F(ApixuTest, DefaultHttpClient) { EXPECT_NO_THROW(apixu::Apixu{""}); }

TEST_F(ApixuTest, NullHttpClient)
{
    EXPECT_THROW((apixu::Apixu{"", nullptr}), std::invalid_argument);

    std::unique_ptr<apixu::http::Http> client;
    EXPECT_THROW((apixu::Apixu{"", std::move(client)}), std::invalid_argument);
}
}  // namespace apixutest
