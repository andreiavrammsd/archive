/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

// C style string type trait
template <typename T>
struct Is_C_style_string : std::false_type {
};

template <>
struct Is_C_style_string<const char *> : std::true_type {
};

template <>
struct Is_C_style_string<char *> : std::true_type {
};

// std string type trait
template <typename T>
using Is_string = std::is_same<T, std::string>;

// Printf implementation
void Printf(const char *s)
{
    if (s == nullptr) {
        return;
    }

    while (*s) {
        if (*s == '%' && *++s != '%') {  // make sure no more arguments are expected
            // %% represents plain % in a format string
            throw std::runtime_error("invalid format: missing arguments");
        }

        std::cout << *s++;
    }
}

// variadic template argument list: one or more arguments
template <typename T, typename... Args>
void Printf(const char *s, T value, Args... args)
{  // function argument list: two or more arguments
    while (s && *s) {
        if (*s == '%' && *++s != '%') {  // a format specifier (ignore which one it is)
            switch (*s) {
                case 's':
                    if (!Is_C_style_string<T>() && !Is_string<T>()) {
                        throw std::runtime_error("Bad Printf() format");
                    }
                    break;
                case 'd':
                    if (!std::is_integral<T>()) {
                        throw std::runtime_error("Bad Printf() format");
                    }
                    break;
                case 'g':
                    if (!std::is_floating_point<T>()) {
                        throw std::runtime_error("Bad Printf() format");
                    }
                    break;
            }

            std::cout << value;           // user first non-format argument
            return Printf(++s, args...);  // do a recursive call with the tail of the argument list
        }

        std::cout << *s++;
    }

    throw std::runtime_error("extra arguments provided to Printf");
}

// Testing
using TestRunner = void (*)();

struct Test {
    explicit Test(TestRunner r, const char *e) : runner{r}, expectedResult{e}
    {
        orig_buf = std::cout.rdbuf(out.rdbuf());
    }

    void operator()()
    {
        try {
            runner();

            if (expectedResult == nullptr) {
                std::cerr << "exception expected" << std::endl;
                return;
            }
        }
        catch (...) {
            return;
        }

        if (out.str() != expectedResult) {
            std::cerr << "got \"" << out.str() << "\"; expected \"" << expectedResult << "\"" << std::endl;
        }
    }

    ~Test() { std::cout.rdbuf(orig_buf); }

   private:
    TestRunner runner;
    const char *expectedResult;
    std::ostringstream out;
    std::streambuf *orig_buf;
};

int main()
{
    std::map<TestRunner, const char *> tests = {
        {[] { Printf(""); }, ""},
        {[] { Printf(" "); }, " "},
        {[] { Printf("%s=%d", "X", 2); }, "X=2"},
        {[] { Printf("%s=%s", "X", "1"); }, "X=1"},
        {[] { Printf("abc %d%d%d", 1, 2, 3); }, "abc 123"},
        {[] { Printf("%s%g%d text", std::string{"abc"}, 2.2, 3); }, "abc2.23 text"},
        {[] { Printf("%% %s", "X"); }, "% X"},
        {[] { Printf("%%%%"); }, "%%"},
        {[] { Printf("%% %s", 1); }, nullptr},
        {[] { Printf("%g", 1); }, nullptr},
        {[] {
             std::string val = "val";
             Printf("val is %s", val);
         },
         "val is val"},
        {
            [] {
                std::string b = "b";
                const char *c = "c";
                Printf("%s%s%s%s", "a", b, c, "d");
            },
            "abcd",
        }};

    for (const auto &t : tests) {
        Test test(t.first, t.second);
        test();
    }
}
