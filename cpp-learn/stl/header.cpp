#include <utility>

#include "header.h"

Header::Header::Header(std::string in) noexcept : in(std::move(in)) {

}

std::string Header::Header::concat(const std::string &s) {
    if (s.empty()) {
        throw EmptyInput{};
    }

    return in + s;
}
