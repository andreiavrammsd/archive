/*
 The C++ Programming Language, 4th Edition
 */

#include <sstream>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>

template <typename T>
struct Named_val {
    std::string name;
    T value;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Named_val<T> &nv)
{
    return os << '{' << nv.name << ':' << nv.value << '}';
}

class My_base {
   public:
    virtual std::ostream &put(std::ostream &s) const = 0;  // write *this to s
};

std::ostream &operator<<(std::ostream &s, const My_base &r)
{
    return r.put(s);  // use the right put()
}

class Sometype : public My_base {
    int i;

   public:
    explicit Sometype(int i) : i{i} {}

    std::ostream &put(std::ostream &s) const override
    {  // the real output function
        return s << i;
    }
};

void f(const My_base &r, Sometype &s) { std::cout << r << s; }

struct smanip {
    std::ios_base &(*f)(std::ios_base &, int);  // function to be called
    int i;

    smanip(std::ios_base &(*ff)(std::ios_base &, int), int ii) : f{ff}, i{ii} {}
};

template <typename C, typename Tr>
std::basic_ostream<C, Tr> &operator<<(std::basic_ostream<C, Tr> &os, const smanip &m)
{
    m.f(os, m.i);  // call m's f with m's stored value
    return os;
}

inline smanip setprecision(int n)
{
    auto h = [](std::ios_base &s, int x) -> std::ios_base & {
        s.precision(x);
        return s;
    };

    return smanip(h, n);  // make the function object
}

template <typename T>
char *as_bytes(T &i)
{
    return static_cast<char *>(&i);  // treat that memory as bytes
}

void test()
{
    std::string input = "^Cn^C^Z^A^@^@^@A^@^E^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@W^@";
    std::string expected = "_Do_D_[_B_A_A_AB_A_F_A_A_A_A_A_A_A_A_A_A_A_A_A_A_A_A_A_AX_A";

    std::istringstream iss(std::ios_base::binary);  // stream mode is binary
    std::ostringstream oss(std::ios_base::binary);  // stream mode is binary

    iss.str(input);

    std::vector<char> v;

    for (char i; iss.read(as_bytes(i), sizeof(i));) {  // read bytes from binary file
        v.push_back(i);
    }

    // ... do something with v ...
    for (auto &i : v) {
        ++i;
    }

    for (auto i : v) {  // write bytes to binary file
        oss.write(as_bytes(i), sizeof(i));
    }

    assert(oss.str() == expected);
}

class Form;  // our formatting type

struct Bound_form {  // Form plus value
    const Form &f;
    double val;
};

class Form {
    friend std::ostream &operator<<(std::ostream &, const Bound_form &);

    int prc;                      // precision
    int wdt;                      // width 0 means "as wide as necessary"
    std::ios_base::fmtflags fmt;  // general, scientific, or fixed (§38.4.5.1)

   public:
    // std::ios_base::fmtflags
    explicit Form(int p = 6, std::ios_base::fmtflags f = std::ios_base::fmtflags(0), int w = 0) : prc{p}, wdt{w}, fmt{f}
    {
    }

    Bound_form operator()(double d) const
    {  // make a Bound_form for *this and d
        return Bound_form{*this, d};
    }

    Form &scientific()
    {
        fmt = std::ios_base::scientific;
        return *this;
    }

    Form &precision(int p)
    {
        prc = p;
        return *this;
    }

    Form &width(int w)
    {
        wdt = w;
        return *this;
    }
};

std::ostream &operator<<(std::ostream &os, const Bound_form &bf)
{
    std::ostringstream s;
    s.precision(bf.f.prc);
    s.setf(bf.f.fmt, std::ios_base::floatfield);
    s << bf.val;  // compose string s
    return os << s.str();
}

void f(double d)
{
    Form gen4{4};  // general format, precision 4

    Form sci8;
    sci8.scientific().precision(8);  // scientific format, precision 8
    std::cout << d << ' ' << gen4(d) << ' ' << sci8(d) << ' ' << d << '\n';

    Form sci10{10, std::ios_base::scientific};  // scientific format, precision 10
    std::cout << d << ' ' << gen4(d) << ' ' << sci10(d) << ' ' << d << '\n';
}

int main()
{
    /**
     * Input operations
     *
     * Input operations are provided by istream (§38.6.2), found in <istream> except for the ones reading
     * into a string; those are found in <string>. The basic_istream is primarily intended as a base class for
     * more specific input classes, such as istream and istringstream:
     */

    std::stringstream stream{"1 \na\n2\n"};

    // Formatted Input
    // Formatted input is primarily supplied by the >> ("input," "get," or "extraction") operator:

    // in>>x
    //
    // Read from in into x according to x's type; x can be an arithmetic type,
    // a pointer, a basic_string, a valarray , a basic_streambuf ,
    // or any type for which the user has supplied a suitable operator>>()
    //
    // By default >> skips whitespace

    int i;
    std::string str;

    stream >> i;
    assert(i == 1);

    stream >> i;
    assert(i == 0);  // 'a'

    stream.clear();
    stream.seekg(0, std::ios_base::beg);
    stream >> str;
    assert(str == "1");

    stream.str(" 3");
    i = 0;
    stream >> i;
    assert(i == 3);

    stream.str(" 3");
    i = 0;
    stream >> std::noskipws >> i;
    assert(i == 0);

    // getline(in,s)
    // Read a line from in into the string s

    stream.clear();
    stream.seekg(0, std::ios_base::beg);

    stream.str("1 \na\n2\n");

    getline(stream, str);
    assert(str == "1 ");

    getline(stream, str);
    assert(str == "a");

    // Unformatted Input
    // Unformatted input can be used for finer control of reading and potentially for improved
    // performance. One use of unformatted input is the implementation of formatted input:

    // x=in.get()
    // in.get(c)
    // in.get(p,n,t)
    // in.get(p,n)
    // in.getline(p,n,t)
    // in.getline(p,n)
    // in.read(p,n)
    // x=in.gcount()
    // in.putback(c)
    // in.unget()
    // in.ignore(n,d)
    // in.ignore(n)
    // in.ignore()
    // in.swap(in2)
    // x=in.peek()
    // n=in.readsome(p,n)
    // x=in.sync()
    // pos=in.tellg()
    // in.seekg(pos)
    // in.seekg(off,dir)

    std::stringstream ss{"abcd"};

    auto c = ss.get();
    assert(c == 'a');

    assert(ss.gcount() == 1);
    ss.putback(c);
    assert(ss.gcount() == 0);

    assert(ss.get() == 'a');
    assert(ss.get() == 'b');

    ss.unget();
    assert(ss.get() == 'b');

    ss.ignore();
    assert(ss.get() == 'd');

    ss.seekg(0);
    char vc[2];
    ss.read(vc, 2);
    assert(vc[0] == 'a');
    assert(vc[1] == 'b');

    assert(ss.peek() == 'c');
    assert(ss.get() == 'c');

    /**
     * Output Operations
     *
     * Output operations are provided by ostream (§38.6.1), found in <ostream>
     * except for the ones writing out a string; those are found in <string>
     */

    // out<<x
    // out.put(c)
    // out.write(p,n)
    // out.flush()
    // pos=out.tellp()
    // out.seekp(pos)
    // out.seekp(off,dir)

    std::ostringstream oss;
    std::string data = "abc";

    oss << data;
    oss.put('d');
    oss.write(data.c_str(), 2);
    assert(oss.str() == "abcdab");

    oss.str("");
    oss << Named_val<int>{"a", 1};
    assert(oss.str() == "{a:1}");

    // Virtual Output Functions
    Sometype st1{1};
    Sometype st2{2};
    f(st1, st2);
    std::cout << "\n\n";

    /**
     * Manipulators
     */

    std::cout << std::setprecision(4) << 22.23551 << ' ' << 22.61231 << '\n';
    std::cout << setprecision(4) << 22.23551 << ' ' << 22.61231 << '\n';

    /**
     * Stream State
     *
     * In <ios>, the standard library defines the base class ios_base
     * defining most of the interface to a stream class
     *
     * The basic_ios class manages the state of a stream:
     *      The mapping between a stream and its buffers (§38.6)
     *      The formatting options (§38.4.5.1)
     *      The use of locale s (Chapter 39)
     *      Error handling (§38.3)
     *      Connections to other streams and stdio (§38.4.4)
     */

    // Fundamental ios_base Operations

    // ios_base b {};
    // ios.~ios_base()
    // b2=sync_with_stdio(b)
    // b=sync_with_stdio()

    // A call of sync_with_stdio(true) before the first iostream operation
    // in the execution of a program guarantees that the iostream and stdio (§43.3)
    // I/O operations share buffers.
    // A call of sync_with_stdio(false) before the first stream I/O operation prevents
    // buffer sharing and can improve I/O performance significantly on some implementations.

    // iostate Constants (std::ios_base::)

    // badbit		Something unexpected and serious happened (e.g., a disk read error)
    // failbit		Something unexpected happened (e.g., we looked for a digit and found 'x')
    // eofbit		We hit end-of-input (e.g., end-of-file)
    // goodbit		All is well

    // Functions for reading these bits

    // bad()
    // fails()
    // eof()
    // good()

    // openmode Constants (std::ios_base::)

    // app      Append (insert output at end-of-stream)
    // ate		At end (position to end-of-stream)
    // binary	Don't apply formatting to characters
    // // The exact meaning of ios_base::binary is implementation-dependent. However, the usual meaning
    // is that a character gets mapped to a byte.
    // in       Input stream
    // out		Output stream
    // trunc	Truncate stream before use (set the stream's size to zero)

    // Use binary I/O when dealing with objects that are ""just bags of bits and do not have an obvious
    // and reasonable character string representation. Images and sound/video streams are examples.
    test();

    // seekdir Constants (std::ios_base::)

    // beg
    // cur
    // end

    // Classes derived from basic_ios format output and extract objects based on the information stored in
    // their basic_io. The ios_base operations can be summarized:

    // basic_ios ios {p};
    // ios.~basic_ios()
    // bool b {ios};
    // b=!ios
    // st=ios.rdstate()
    // ios.clear(st)
    // ios.clear()
    // ios.setstate(st)
    // ios.good()
    // ios.eof()
    // ios.fail()
    // ios.bad()
    // st=ios.exceptions()
    // ios.exceptions(st)
    // p=ios.tie()
    // p=ios.tie(os)
    // p=ios.rdbuf()
    // p=ios.rdbuf(p2)
    // ios3=ios.copyfmt(ios2)
    // c=ios.fill()
    // c2=ios.fill(c)
    // loc2=ios.imbue(loc)
    // c2=narrow(c,d)
    // c2=widen(c)
    // ios.init(p)
    // ios.set_rdbuf(p)
    // ios.move(ios2)
    // ios.swap(ios2)

    // The tie() is used to ensure that output from a tied stream appears before an input from the stream to
    // which it is tied. For example, cout is tied to cin:

    // ios_base Operations

    // i=xalloc()
    // r=iob.iword(i)
    // r=iob.pword(i)
    // iob.register_callback(fn,i)

    std::cout << '\n';

    auto callback = [](std::ios_base::event e, std::ios_base &, int index) {
        std::string event;
        switch (e) {
            case std::ios_base::event::erase_event:
                event = "erase";
                break;
            case std::ios_base::imbue_event:
                event = "imbue";
                break;
            case std::ios_base::copyfmt_event:
                event = "copyfmt";
                break;
        }
        std::cout << "event: " << event << "@" << index << '\n';
    };

    oss.register_callback(callback, 0);
    oss.register_callback(callback, 1);

    std::cout << '\n';

    /**
     * Formatting
     */

    // Formatting state

    // boolalpha
    // dec
    // hex
    // oct
    // fixed
    // scientific
    // internal
    // left
    // right
    // showbase
    // showpoint
    // showpos
    // skipws
    // unitbuf
    // uppercase
    // adjustfield
    // basefield
    // floatfield

    // Formatting fmtflags Operations

    // f=ios.flags()
    // f2=ios.flags(f)
    // f2=ios.setf(f)
    // f2=ios.setf(f,m)
    // ios.unsetf(f)
    // n=ios.precision()
    // n2=ios.precision(n)
    // n=ios.width()
    // n2=ios.width(n)

    oss.clear();
    oss.str("");

    oss << true;
    assert(oss.str() == "1");

    oss.setf(std::ios_base::boolalpha);
    oss << true;
    assert(oss.str() == "1true");

    // locale Operations

    // loc2=ios.imbue(loc)
    // loc=ios.getloc()

    // <ios> Standard Manipulators

    // s<<boolalpha
    // s<<noboolalpha
    // s<<showbase
    // s<<noshowbase
    // s<<showpoint
    // s<<noshowpoint
    // s<<showpos
    // s<<noshowpos
    // s<<uppercase
    // s<<nouppercase
    // s<<unitbuf
    // s<<nounitbuf
    // s<<internal
    // s<<left
    // s<<right
    // s<<dec
    // s<<hex
    // s<<oct
    // s<<fixed
    // s<<scientific
    // s<<hexfloat
    // s<<defaultfloat
    // s>>skipws
    // s>>noskipws

    // <ostream> Standard Manipulators

    // os<<endl
    // os<<ends
    // os<<flush

    // Each of these operations returns a reference to its first (stream) operand, s.
    std::cout << 1 << ',' << std::ios_base::hex << 2 << ',' << std::ios_base::oct << 3 << '\n';

    constexpr double d = 123.456;

    std::cout << d << ";" << std::ios_base::scientific << d << ";" << std::ios_base::fixed << d << ";"
              << std::ios_base::floatfield << d << "\n\n";

    // <iomanip> Standard Manipulators

    // s<<resetiosflags(f)
    // s<<setiosflags(f)
    // s<<setbase(b)
    // s<<setfill(int c)
    // s<<setprecision(n)
    // s<<setw(n)
    // is>>get_money(m,intl)
    // is>>get_money(m)
    // os<<put_money(m,intl)
    // os<<put_money(m)
    // is>>get_time(tmp,fmt)
    // os<<put_time(tmp,fmt)

    std::cout << '(' << std::setw(4) << 12 << ")(" << 12 << ")\n\n";

    // <istream> Standard Manipulators

    // s>>skipws
    // s>>noskipws
    // is>>ws

    std::string input{"0 1 2 3 4"};
    std::istringstream iss{input};
    std::string s;
    for (char ch; iss >> ch;) {
        s += ch;
    }
    assert(s == "01234");

    s = "";
    std::istringstream iss2{input};
    iss2 >> std::noskipws;
    for (char ch; iss2 >> ch;) {
        s += ch;
    }
    assert(s == "0 1 2 3 4");

    // User-Defined Manipulators
    f(1234.56789);
}
