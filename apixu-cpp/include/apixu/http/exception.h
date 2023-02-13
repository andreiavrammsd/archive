// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_HTTP_EXCEPTION_H_
#define INCLUDE_APIXU_HTTP_EXCEPTION_H_

#include <exception>
#include <string>

namespace apixu {
namespace http {
class Exception : public std::exception {
   public:
    explicit Exception(const std::string& msg) : msg_(msg) {}

    const char* what() const noexcept override { return msg_.c_str(); }

   private:
    const std::string msg_;
};
}  // namespace http
}  // namespace apixu

#endif  // INCLUDE_APIXU_HTTP_EXCEPTION_H_
