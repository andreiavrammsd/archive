// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_EXCEPTION_APIXU_EXCEPTION_H_
#define INCLUDE_APIXU_EXCEPTION_APIXU_EXCEPTION_H_

#include <exception>
#include <string>
#include <utility>

namespace apixu {
namespace exception {
class ApixuException : public std::exception {
   public:
    explicit ApixuException(const std::string& msg) : msg_(msg) {}

    const char* what() const noexcept override { return msg_.c_str(); }

   private:
    const std::string msg_;
};
}  // namespace exception
}  // namespace apixu

#endif  // INCLUDE_APIXU_EXCEPTION_APIXU_EXCEPTION_H_
