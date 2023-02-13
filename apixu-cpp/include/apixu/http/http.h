// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_HTTP_HTTP_H_
#define INCLUDE_APIXU_HTTP_HTTP_H_

#include <curl/curl.h>

#include <cstdint>
#include <map>
#include <sstream>
#include <string>
#include <utility>

namespace apixu {
namespace http {
enum class Status {
    Ok = 200,
    InternalServerError = 500,
    BadRequest = 400,
};

struct Response {
    Status status;
    std::string body;
};

using Parameters = std::map<std::string, std::string>;

class Http {
   public:
    virtual Response Get(const std::string& path, const Parameters& params) const = 0;
    virtual ~Http() = default;
};
}  // namespace http
}  // namespace apixu

#endif  // INCLUDE_APIXU_HTTP_HTTP_H_
