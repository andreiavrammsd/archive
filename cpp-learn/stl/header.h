#ifndef HEADER
#define HEADER

#include <string>

namespace Header {
    using std::string;

    class Header {
    public:
        explicit Header(std::string in) noexcept;

        string concat(const string &);

    private:
        const string in;
    };

    struct EmptyInput : public std::exception {
    };
}

#endif
