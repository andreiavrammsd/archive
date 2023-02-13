// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_EXCEPTION_API_EXCEPTION_H_
#define INCLUDE_APIXU_EXCEPTION_API_EXCEPTION_H_

#include <string>

#include "./apixu_exception.h"

namespace apixu {
namespace exception {
class ApiException : public ApixuException {
   public:
    ApiException(const std::string& message, int code) : ApixuException(message), code_(code) {}

    int getCode() const { return code_; }

   private:
    int code_;
};
}  // namespace exception
}  // namespace apixu

#endif  // INCLUDE_APIXU_EXCEPTION_API_EXCEPTION_H_
