/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <locale>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <fstream>
#include <codecvt>

enum Season {
    spring, summer, fall, winter
};

class Season_io : public std::locale::facet {
public:
    Season_io(size_t i = 0) : std::locale::facet{i} {}

    ~Season_io() = default;

    virtual const std::string &to_str(Season x) const = 0; // string representation of x
    virtual bool from_str(const std::string &s, Season &x) const = 0; // place Season for s in x

    static std::locale::id id;
};

std::locale::id Season_io::id{};

std::ostream &operator<<(std::ostream &os, Season x);

std::istream &operator>>(std::istream &is, Season &x);

class US_season_io : public Season_io {
    static const std::string seasons[];
public:
    const std::string &to_str(Season) const;

    bool from_str(const std::string &, Season &) const;

    // note: no US_season_id::id
};

void sort_with_locale(std::vector<std::string> &v, const std::locale &loc);

int string_comparison(const std::string &s1, const std::string &s2, const std::collate<char> &cmp);

struct Money {
    using Value = long long;
    Value amount{};

    operator long long() const { return amount; }

    Money &operator=(long long i) {
        amount = i;
        return *this;
    }
};

template<class C, class Tr>
void handle_ioexception(std::basic_ios<C, Tr> &s); // caled from catch-clause

std::ostream &operator<<(std::ostream &s, Money m);

std::istream &operator>>(std::istream &s, Money &m);

enum class Month {
    jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
};

class Date {
public:
    int d;
    Month m;
    int year;

    explicit Date(int d = {}, Month m = {}, int year = {}) : d{d}, m{m}, year{year} {}

    std::string to_string(const std::locale &loc = std::locale()) const;
};

std::istream &operator>>(std::istream &is, Date &d);

std::ostream &operator<<(std::ostream &os, Date d);

void create_catalog(const std::string &name, const std::locale &loc);

struct Set {
    std::vector<std::string> msgs;
};

struct Cat {
    std::vector<Set> sets;
};

class My_messages : public std::messages<char> {
    std::vector<Cat> &catalogs;
public:
    explicit My_messages(size_t = 0) : catalogs{*new std::vector<Cat>} {}

    std::messages_base::catalog do_open(const std::string &s, const std::locale &loc) const override; // open catalog s

    std::string
    do_get(std::messages_base::catalog cat, int set, int id,
           const std::string &def) const override; // get messages (set,id) in cat

    void do_close(std::messages_base::catalog cat) const override {
        if (catalogs.size() <= static_cast<size_t>(cat)) {
            catalogs.erase(catalogs.begin() + cat);
        }
    }

    ~My_messages() override {
        delete &catalogs;
    }
};

int main() {
    /**
     * Class locale
     */

    // locale loc1;                         // copy of the current global locale
    // locale loc2 {""};                    // copy of "the user's preferred locale"
    // locale loc3 {"C"};                   // copy of the "C" locale
    // locale loc4 {locale::classic()};     // copy of the "C" locale
    // locale loc5 {"POSIX"};               // copy of the locale named "POSIX"
    // locale loc6 {"Danish_Denmark.1252"}; // copy of the locale named "Danish_Denmark.1252"
    // locale loc7 {"en_US.UTF−8"};         // copy of the locale named "en_US.UTF-8"

    std::locale previous_locale;
    try {
        previous_locale = std::locale::global(std::locale{""});
        std::cout << previous_locale.name() << "\n\n";
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << "\n\n";
    }

    std::cout << "name of current global locale: " << std::locale().name() << "\n";
    std::cout << "name of classic C locale: " << std::locale::classic().name() << "\n";
    std::cout << "name of \"user's preferred locale\": " << std::locale("").name() << "\n";
    std::cout << "\n";

    std::locale my_loc{""};
    // std::locale loc1{std::locale{"POXIS", }, my_loc, std::locale::monetary}; // use monetary facets from loc

    /**
     * Class facet
     *
     * A locale is a collection of facets. A facet represents one specific cultural aspect, such as how a
     * number is represented on output (num_put), how a date is read from input (time_get), and how
     * characters are stored in a file (codecvt).
     */

    // A Simple User-Defined facet
    std::istringstream sin;
    std::ostringstream sout;

    Season x;

    sin.str("2");

    sin >> x;
    sout << x;
    assert(sout.str() == "2");


    std::locale loc(std::locale(), new US_season_io{});
    sout.imbue(loc); // use locale with Season_io facet
    sin.imbue(loc); // use locale with Season_io facet

    sin.clear();
    sin.str("summer");
    sout.clear();
    sout.str("");

    sin >> x;
    sout << x;
    assert(sout.str() == "summer");

    // Standard facets

    // collate      String comparison           collate<C>
    // numeric      Numeric formatting          numpunct<C>, num_g et<C,In>, num_put<C,Out>
    // monetary     Money formatting            moneypunct<C>, moneypunct<C,International>, money_g et<C,In>, money_put<C,Out>
    // time         Date and time formatting    time_put<C,Out>, time_put_byname<C,Out>, time_get<C,In>
    // ctype        Character classification    ctype<C>, codecvt<In,Ex,SS>, codecvt_byname<In,Ex,SS>
    // messages     Message retrieval           messages<C>

    std::locale current{""};

    std::vector<std::string> v = {"b", "a", "c"};
    sort_with_locale(v, current);

    assert(v == (std::vector<std::string>{"a", "b", "c"}));

    // String comparison
    std::string s1 = "az";
    std::string s2 = "zb";

    const std::collate<char> &col{std::use_facet<std::collate<char>>(loc)};
    assert(string_comparison("az", "zb", col) == -1);

    assert(current(s1, s2));

    // Numeric Formatting
    char numeric = std::use_facet<std::numpunct<char>>(current).decimal_point();
    std::cout << "decimal point: " << numeric << "\n";

    // Money formatting
    std::string currency = std::use_facet<std::moneypunct<char>>(current).curr_symbol();
    std::cout.imbue(current);
    std::cout << "money: " << currency << 1.99 << "\n";

    std::istringstream in{"250"};
    std::ostringstream out;

    Money m;
    in >> m;
    out << m;
    assert(out.str() == " 2,50");

    // Date and time formatting
    std::istringstream din{"04/27/2020"};
    din.imbue(previous_locale);

    Date d;

    din >> d;
    std::cout.imbue(previous_locale);
    assert(d.year == 2020);
    assert(d.m == Month::apr);
    assert(d.d == 27);

    // Character classification
    const std::ctype<char> &ctype{std::use_facet<std::ctype<char>>(previous_locale)};

    assert(ctype.is(std::ctype_base::space, ' '));
    assert(ctype.is(std::ctype_base::upper, 'A'));
    assert(ctype.toupper('c') == 'C');
    assert(ctype.widen('c') == 'c');

    // Character Code Conversion
    auto conv = new std::codecvt<wchar_t, char, std::mbstate_t>{}; // standard char to wide char
    std::locale conv_loc{previous_locale, conv};

    std::istringstream convis{"abc"};
    convis.imbue(conv_loc);

    for (char ch; convis >> ch;) {
        std::cout << ch;
    }
    std::cout << "\n\n";

    // Messages
    std::string cat_name = "cat";
    create_catalog(cat_name, previous_locale);

    auto msgs = My_messages{};
    auto cat = msgs.open(cat_name, previous_locale);
    if (cat < 0) {
        throw std::runtime_error("cannot open catalog:" + cat_name);
    }

    assert(msgs.get(cat, 0, 0, "Missed again!") == "hello");
    assert(msgs.get(cat, 1, 2, "Missed again!") == "maybe");
    assert(msgs.get(cat, 1, 3, "Missed again!") == "Missed again!");
    assert(msgs.get(cat, 3, 0, "Missed again!") == "Missed again!");

    /**
     * Convenience Interfaces
     *
     * Beyond simply imbuing an iostream, the locale facilities can be complicated to use. Consequently,
     * convenience interfaces are provided to simplify notation and minimize mistakes.
     */

    // Character Classification
    assert(std::isspace(' ', previous_locale));

    // isblank(c,loc)
    // isprint(c,loc)
    // iscntrl(c,loc)
    // isupper(c,loc)
    // islower(c,loc)
    // isalpha(c,loc)
    // isdigit(c,loc)
    // ispunct(c,loc)
    // isxdigit(c,loc)
    // isalnum(c,loc)
    // isgraph(c,loc)

    // Character Conversions
    assert(std::toupper('a', previous_locale) == 'A');
    assert(std::tolower('A', previous_locale) == 'a');

    // String Conversions
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter{};

    const std::string s8{reinterpret_cast<const char*>(u8"This is a UTF-8 string")};
    std::wstring s16 = converter.from_bytes(s8);
    std::string s88 = converter.to_bytes(s16);
    assert(s8 == s88);

    // wstring_convert cvt {};
    // wstring_convert cvt {pcvt,state}
    // wstring_convert cvt {pcvt};
    // wstring_convert cvt {b_err,w_err};
    // wstring_convert cvt {b_err};
    // cvt.~wstring_conver t();

    // ws=cvt.from_bytes(c)
    // ws=cvt.from_bytes(s)
    // ws=cvt.from_bytes(b,e)
    // s=cvt.to_bytes(wc)
    // s=cvt.to_bytes(ws)
    // s=cvt.to_bytes(b,e)
    // n=cvt.converted()
    // st=cvt.state()

    // Buffer Conversions
    // wbuffer_convert wb {psb,pcvt,state};
    // wbuffer_convert wb {psb,pcvt};
    // wbuffer_convert wb {psb};
    // wbuffer_convert wb {};

    // psb=wb.rdbuf()
    // psb2=wb.rdbuf(psb)
    // t=wb.state()
}


std::ostream &operator<<(std::ostream &os, Season x) {
    std::locale loc{os.getloc()}; // extract the stream's locale

    if (std::has_facet<Season_io>(loc)) {
        return os << std::use_facet<Season_io>(loc).to_str(x); // string representation
    }

    return os << static_cast<int>(x); // integer representation
}

std::istream &operator>>(std::istream &is, Season &x) {
    const std::locale &loc{is.getloc()}; // extract the stream's locale

    if (std::has_facet<Season_io>(loc)) {
        const Season_io &f{std::use_facet<Season_io>(loc)}; // get hold of the locale's Season_io facet

        std::string buf;
        if (!(is >> buf && f.from_str(buf, x))) { // read alphabetic representation
            is.setstate(std::ios_base::failbit);
        }
        return is;
    }

    int i;
    is >> i;
    x = static_cast<Season>(i); // read numeric representation
    return is;
}


const std::string US_season_io::seasons[] = {
        "spring",
        "summer",
        "fall",
        "winter",
};

const std::string &US_season_io::to_str(Season x) const {
    if (x < spring || winter < x) {
        static const std::string ss = "no-such-season";
        return ss;
    }

    return seasons[x];
}

bool US_season_io::from_str(const std::string &s, Season &x) const {
    const std::string *p = find(begin(seasons), end(seasons), s);
    if (p == end(seasons)) {
        return false;
    }

    x = Season(p - begin(seasons));
    return true;
}

void sort_with_locale(std::vector<std::string> &v, const std::locale &loc) {
    const std::collate<char> &col{std::use_facet<std::collate<char>>(loc)};
    const std::ctype<char> &ctyp{std::use_facet<std::ctype<char>>(loc)};

    std::locale loc1{loc, &col}; // use string comparison
    std::locale loc2{loc1, &ctyp}; // use character classification and string comparison

    sort(v.begin(), v.end(), loc2);
}

int string_comparison(const std::string &s1, const std::string &s2, const std::collate<char> &cmp) {
    const char *cs1{s1.data()}; // because compare() operates on char[]s
    const char *cs2{s2.data()};

    return cmp.compare(cs1, cs1 + s1.size(), cs2, cs2 + s2.size());
}


template<class C, class Tr>
void handle_ioexception(std::basic_ios<C, Tr> &s) {
    if (s.exceptions() & std::ios_base::badbit) {
        try {
            s.setstate(std::ios_base::badbit); // might throw basic_ios::failure
        } catch (...) {
            // ... do nothing ...
        }

        throw; // re-throw
    }
    s.setstate(std::ios_base::badbit);
}

std::ostream &operator<<(std::ostream &s, Money m) {
    std::ostream::sentry guard(s);
    if (!guard) return s;

    try {
        const std::money_put<char> &f = std::use_facet<std::money_put<char>>(s.getloc());

        if (m == static_cast<long long>(m)) { // m can be represented as a long long
            if (f.put(s, true, s, s.fill(), m).failed()) {
                s.setstate(std::ios_base::badbit);
            }
        } else {
            std::ostringstream v;
            v << m; // convert to string representation

            if (f.put(s, true, s, s.fill(), v.str()).failed()) {
                s.setstate(std::ios_base::badbit);
            }
        };
    } catch (...) {
        handle_ioexception(s);
    }

    return s;
}

std::istream &operator>>(std::istream &s, Money &m) {
    std::istream::sentry guard(s);
    if (guard)
        try {
            std::ios_base::iostate state = std::ios_base::goodbit;
            std::istreambuf_iterator<char> eos;
            std::string str;

            std::use_facet<std::money_get<char>>(s.getloc()).get(s, eos, true, std::cin, state, str);

            if (state == std::ios_base::goodbit || state == std::ios_base::eofbit) { // set value only if get succeeded
                long long i = stoll(str);
                if (errno == ERANGE) {
                    state |= std::ios_base::failbit;
                } else {
                    m = i; // set value only if conversion to long long succeeded
                }
                s.setstate(state);
            }


        } catch (...) {
            handle_ioexception(s);
        }

    return s;
}

std::istream &operator>>(std::istream &is, Date &d) {
    if (std::istream::sentry guard{is}) {
        std::ios_base::iostate err;
        struct tm t{};

        std::use_facet<std::time_get<char>>(is.getloc()).get_date(is, nullptr, is, err, &t);

        if (err == std::ios_base::goodbit || err == std::ios_base::eofbit) {
            Month m = static_cast<Month>(t.tm_mon + 1);
            d = Date(t.tm_mday, m, t.tm_year + 1900);
        }
        is.setstate(err);
    }

    return is;
}

void create_catalog(const std::string &name, const std::locale &loc) {
    auto content = "<<<\n"
                   "hello\n"
                   "goodbye\n"
                   ">>>\n"
                   "<<<\n"
                   "yes\n"
                   "no\n"
                   "maybe\n"
                   ">>>\n";

    auto file = name + loc.name();

    std::ofstream cat(file);
    if (!cat) {
        throw std::runtime_error("cannot open file:" + file);
    }

    cat << content;
    if (!cat.good()) {
        throw std::runtime_error("cannot write to file:" + file);
    }

    cat.close();
    if (!cat.good()) {
        throw std::runtime_error("cannot close file:" + file);
    }
}


std::string My_messages::do_get(std::messages_base::catalog cat, int set, int id, const std::string &def) const {
    if (catalogs.size() <= static_cast<size_t>(cat)) {
        return def;
    }

    Cat &c = catalogs[cat];
    if (c.sets.size() <= static_cast<size_t>(set)) {
        return def;
    }

    Set &s = c.sets[set];
    if (s.msgs.size() <= static_cast<size_t>(id)) {
        return def;
    }

    return s.msgs[id];
}

std::messages<char>::catalog My_messages::do_open(const std::string &s, const std::locale &loc) const {
    std::string nn = s + loc.name();
    std::ifstream f(nn.c_str());
    if (!f) {
        return -1;
    }

    catalogs.push_back(Cat{}); // make in-core catalog
    Cat &c = catalogs.back();

    for (std::string str; f >> str && str == "<<<";) { // read Set
        c.sets.push_back(Set{});

        f >> std::ws; // skip rest of the line

        Set &ss = c.sets.back();
        while (getline(f, str) && str != ">>>") { // read message
            ss.msgs.push_back(str);
        }
    }

    return catalogs.size() - 1;
}
