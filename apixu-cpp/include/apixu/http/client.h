// Copyright 2020 <Andrei Avram>
#ifndef INCLUDE_APIXU_HTTP_CLIENT_H_
#define INCLUDE_APIXU_HTTP_CLIENT_H_

#include <curl/curl.h>

#include <cstdint>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "exception.h"
#include "http.h"

namespace apixu {
namespace http {

class Client : public Http {
   public:
    explicit Client(std::string user_agent) : user_agent_(std::move(user_agent)) {}

    Response Get(const std::string& url, const Parameters& params) const override
    {
        CURL* curl = curl_easy_init();
        if (!curl) {
            throw Exception("Cannot init curl");
        }

        std::string api_url = url + "?" + paramsToQuery(curl, params);

        curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent_.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

        std::string read_buffer;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw Exception(curl_easy_strerror(res));
        }

        int response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        curl_easy_cleanup(curl);

        return Response{static_cast<Status>(response_code), read_buffer};
    }

   private:
    std::string user_agent_;

    static std::string paramsToQuery(CURL* curl, const Parameters& params)
    {
        std::ostringstream query;

        for (auto iter = params.begin(); iter != params.end();) {
            char* key = curl_easy_escape(curl, iter->first.c_str(), iter->first.length());
            char* value = curl_easy_escape(curl, iter->second.c_str(), iter->second.length());
            if (key == nullptr || value == nullptr) {
                throw Exception("Cannot escape query params");
            }

            query << key << "=" << value;
            curl_free(key);
            curl_free(value);

            if (next(iter) != params.end()) {
                query << "&";
            }
            ++iter;
        }

        return query.str();
    }

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userdata)
    {
        (static_cast<std::string*>(userdata))->append(static_cast<char*>(contents), size * nmemb);
        return size * nmemb;
    }
};
}  // namespace http
}  // namespace apixu

#endif  // INCLUDE_APIXU_HTTP_CLIENT_H_
