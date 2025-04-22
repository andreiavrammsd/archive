/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <string>
#include <ostream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <cassert>

namespace Chrono {
    enum class Month {
        jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
    };

    using std::string;
    using std::ostream;
    using std::istream;
    using std::ostringstream;

    class Date {
    public: // public interface
        class Bad_date : public std::exception {
        }; // exception class

        explicit Date(int dd = {}, Month mm = {}, int yy = {}); // {} means "pick a default"

        // nonmodifying functions for examining the Date:
        int day() const;

        Month month() const;

        int year() const;

        string string_rep() const; // string representation
        void char_rep(char s[], int max) const; // C-style string representation

        // (modifying) functions for changing the Date:
        Date &add_year(int n); // add n years
        Date &add_month(int n); // add n months
        Date &add_day(int n); // add n days

    private:
        bool is_valid(); // check if this Date represents a date
        int d; // representation
        Month m; // representation
        int y; // representation
    };

    bool is_date(int d, Month m, int y); // true for valid date
    bool is_leapyear(int y); // true if y is a leap year

    bool operator==(const Date &a, const Date &b);

    bool operator!=(const Date &a, const Date &b);

    const Date &default_date(); // the default date

    ostream &operator<<(ostream &os, const Date &d); // print d to os
    istream &operator>>(istream &is, Date &d); // read Date from is into d
} // Chrono

namespace Chrono {
    Date::Date(int dd, Chrono::Month mm, int yy)
            : d{dd}, m{mm}, y{yy} {
        if (y == 0) {
            y = default_date().year();
        }

        if (m == Month{}) {
            m = default_date().month();
        }

        if (d == 0) {
            d = default_date().day();
        }

        if (!is_valid()) {
            throw Bad_date();
        }
    }

    inline int Date::day() const {
        return d;
    }

    inline Month Date::month() const {
        return m;
    }

    inline int Date::year() const {
        return y;
    }

    string Date::string_rep() const {
        ostringstream str;
        str << y << "-";

        int mm = static_cast<int>(m);
        if (mm < 10) {
            str << 0;
        }

        str << mm << "-" << d;

        return str.str();
    }

    const Date &default_date() {
        static Date d{1, Month::jan, 1960};
        return d;
    }

    bool Date::is_valid() {
        return is_date(d, m, y);
    }

    bool is_date(int d, Month m, int y) {
        int ndays;

        switch (m) {
            case Month::feb:
                ndays = 28 + is_leapyear(y);
                break;
            case Month::apr:
            case Month::jun:
            case Month::sep:
            case Month::nov:
                ndays = 30;
                break;
            case Month::jan:
            case Month::mar:
            case Month::may:
            case Month::jul:
            case Month::aug:
            case Month::oct:
            case Month::dec:
                ndays = 31;
                break;
            default:
                return false;
        }

        return 1 <= d && d <= ndays;
    }

    bool is_leapyear(int y) {
        return ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0);
    }

    bool operator==(const Date &a, const Date &b) {
        return a.day() == b.day() && a.month() == b.month() && a.year() == b.year();
    }

    ostream &operator<<(ostream &os, const Date &d) {
        os << d.string_rep();
        return os;
    }
}

int main() {
    using namespace Chrono;

    try {
        Date{29, Month::feb, 2019};
    } catch (const Date::Bad_date &) {
        std::cout << "bad date" << std::endl;
    }

    Date d{28, Month::feb, 2019};
    Date d2{28, Month::feb, 2019};

    std::cout << (d == d2) << std::endl;

    std::cout << d << std::endl;

    using namespace std::chrono_literals; // since C++14
    std::chrono::duration s = 1s;
    auto min = s + 59s;
    assert(min == 1min);

    std::chrono::duration duration = 1025.5s;
    auto ns = std::chrono::round<std::chrono::seconds>(duration); // since C++17
    assert(ns == 1026s);
}
