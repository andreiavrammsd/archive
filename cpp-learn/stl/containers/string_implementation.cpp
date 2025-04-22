/*
 The C++ Programming Language, 4th Edition
 */

#include <stdexcept>
#include <cstring>
#include <ostream>
#include <istream>
#include <cassert>
#include <sstream>

using std::ostream;
using std::istream;
using std::stringstream;
using std::ws;

class String {
public:
    String(); // default constructor: x{""}

    explicit String(const char *p); // constructor from C-style string: x{"Euler"}

    String(const String &); // copy constructor
    String &operator=(const String &); // copy assignment

    String(String &&) noexcept; // move constructor
    String &operator=(String &&) noexcept; // move assignment

    ~String() { // destructor
        if (short_max < sz) {
            delete[] ptr;
        }
    }

    char &operator[](int n) { // unchecked element access
        return ptr[n];
    }

    char operator[](int n) const {
        return ptr[n];
    }

    char &at(int n) { // range-checked element access
        check(n);
        return ptr[n];
    }

    char at(int n) const {
        check(n);
        return ptr[n];
    }

    String &operator+=(char c); // add c at the end
    String &operator+=(char *c);

    String &operator=(const char *) noexcept;

    char *c_str() { // C-style string access
        return ptr;
    }

    const char *c_str() const {
        return ptr;
    }

    size_t size() const { // number of elements
        return sz;
    }

    size_t capacity() const { // elements plus available space
        return (sz <= short_max) ? short_max : sz + space;
    }

private:
    static const size_t short_max = 15;
    size_t sz{}; // number of characters
    char *ptr{};
    union {
        size_t space{}; // unused allocated space
        char ch[short_max + 1]; // leave space for terminating 0
    };

    inline void check(size_t n) const { // range check
        if (sz <= n) {
            throw std::out_of_range("String::at()");
        }
    }

    // ancillary member functions:

    void copy_from(const String &x);

    void move_from(String &x) noexcept;
};

char *expand(const char *ptr, size_t n) { // expand into free store
    char *p = new char[n];
    strcpy(p, ptr);
    return p;
}

String::String() // default constructor: x{""}
        : sz{0}, ptr{ch} { // ptr points to elements, ch is an initial location (§19.3.3)
    ch[0] = 0; // terminating 0
}

String::String(const char *p)
        : sz{strlen(p)},
          ptr{(sz <= short_max) ? ch : new char[sz + 1]},
          space{0} {
    strcpy(ptr, p); // copy characters into ptr from p
}

String::String(const String &x) { // copy constructor
    copy_from(x); // copy representation from x
}

String::String(String &&x) noexcept { // move constructor
    move_from(x);
}

String &String::operator=(const String &x) {
    if (this == &x) { // deal with self-assignment
        return *this;
    }

    char *p = (short_max < sz) ? ptr : nullptr;
    copy_from(x);
    delete[] p;

    return *this;
}

String &String::operator=(String &&x) noexcept {
    if (this == &x) { // deal with self-assignment (x = move(x) is insanity)
        return *this;
    }

    if (short_max < sz) { // delete target
        delete[] ptr;
    }

    move_from(x); // does not throw

    return *this;
}

String &String::operator+=(char c) {
    if (sz == short_max) { // expand to long string
        size_t n = sz + sz + 2; // double the allocation (+2 because of the terminating 0)
        ptr = expand(ptr, n);
        space = n - sz - 2;
    } else if (short_max < sz) {
        if (space == 0) { // expand in free store
            size_t n = sz + sz + 2; // double the allocation (+2 because of the terminating 0)
            char *p = expand(ptr, n);
            delete[] ptr;
            ptr = p;
            space = n - sz - 2;
        } else {
            --space;
        }
    }

    ptr[sz] = c; // add c at end
    ptr[++sz] = 0; // increase size and set terminator

    return *this;
}

String &String::operator+=(char *c) {
    while (*c != 0) {
        *this += *c;
        ++c;
    }

    return *this;
}

String &String::operator=(const char *c) noexcept {
    if (short_max < sz) {
        delete[] ptr;
    }

    if (strlen(c) <= short_max) { // copy *this
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdynamic-class-memaccess"
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif

        memcpy(this, c, sizeof(*c));

#ifdef __clang__
#pragma clang diagnostic pop
#elif __GNUC__
#pragma GCC diagnostic pop
#endif
        ptr = ch;
    } else { // copy the elements
        ptr = expand(c, strlen(c) + 1);
        sz = strlen(c);
        space = 0;
    }

    return *this;
}

void String::copy_from(const String &x) { // make *this a copy of x
    if (x.sz <= short_max) { // copy *this

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdynamic-class-memaccess"
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif

        memcpy(this, &x, sizeof(x));

#ifdef __clang__
#pragma clang diagnostic pop
#elif __GNUC__
#pragma GCC diagnostic pop
#endif

        ptr = ch;
    } else { // copy the elements
        ptr = expand(x.ptr, x.sz + 1);
        sz = x.sz;
        space = 0;
    }
}

void String::move_from(String &x) noexcept {
    if (x.sz <= short_max) { // copy *this

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdynamic-class-memaccess"
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif

        memcpy(this, &x, sizeof(x));

#ifdef __clang__
#pragma clang diagnostic pop
#elif __GNUC__
#pragma GCC diagnostic pop
#endif

        ptr = ch;
    } else { // grab the elements
        ptr = x.ptr;
        sz = x.sz;
        space = x.space;
        x.ptr = x.ch; // x = ""
        x.sz = 0;
        x.ch[0] = 0;
    }
}

ostream &operator<<(ostream &os, const String &s) {
    return os << s.c_str();
}

istream &operator>>(istream &is, String &s) {
    s = "";
    is >> ws;
    char ch = ' ';
    while (is.get(ch) && !isspace(ch)) {
        s += ch;
    }

    return is;
}

bool operator==(const String &a, const String &b) {
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i != a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

bool operator==(const String &a, const char *b) {
    if (a.size() != strlen(b)) {
        return false;
    }

    for (size_t i = 0; i != a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const String &a, const String &b) {
    return !(a == b);
}

bool operator!=(const String &a, const char *b) {
    return !(a == b);
}

char *begin(String &x) {
    return x.c_str();
}

char *end(String &x) {
    return x.c_str() + x.size();
}

const char *begin(const String &x) {
    return x.c_str();
}

const char *end(const String &x) {
    return x.c_str() + x.size();
}

String &operator+=(String &a, const String &b) {
    for (auto x : b) {
        a += x;
    }

    return a;
}

String operator+(const String &a, const String &b) {
    String res{a};
    res += b;
    return res;
}

String operator "" _s(const char *p, size_t) {
    return String{p};
}

String generate() {
    String s{"gh"};
    return s;
}

int main() {
    String s;
    assert(s.size() == 0);
    assert(s.capacity() == 15);

    s += 'a';
    s += 'b';
    assert(s == "ab");
    assert(s == "ab"_s);

    auto p = (char *) "cd";
    s += p;
    assert(s == "abcd");

    String s2{"ef"};
    s += s2;
    assert(s == "abcdef");
    assert(s.size() == 6);
    assert(s.capacity() == 15);

    s = s + s + s + s + s;
    assert(s == "abcdefabcdefabcdefabcdefabcdef");
    assert(s.size() == 30);
    assert(s.capacity() == 49);

    String s3;
    s3 = s;
    assert(s3 == "abcdefabcdefabcdefabcdefabcdef");
    assert(s3.size() == 30);
    assert(s3.capacity() == 30);

    auto s4 = generate();
    assert(s4 == "gh");
    assert(s4.size() == 2);
    assert(s4.capacity() == 15);

    stringstream ss;
    ss << s4;
    assert(ss.str() == "gh");

    assert(s4[0] == 'g');
    assert(s4[1] == 'h');
    assert(s4.at(0) == 'g');
    assert(s4.at(1) == 'h');
    try {
        s4.at(2);
        assert(false);
    } catch (...) {
        static_assert(true, "range checked");
    }

    String s5;
    for (const auto c : s4) {
        s5 += c;
    }
    assert(s5 == s4);

    assert(s3 != s5);

    stringstream is;
    is << "abc";
    String buf;
    is >> buf;
    assert(buf == "abc");

    buf = ""_s;
    assert(buf == "");

    s3 = "abcdeabcdeabcdeabcde";
    assert(s3 == "abcdeabcdeabcdeabcde");

    s3 = "";
    assert(s3 == "");

    const char *str = s5.c_str();
    assert(strcmp(str, "gh") == 0);
}
