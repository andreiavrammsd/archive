/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

int main() {
    /**
     * Standard I/O Streams
     */

    // cout     The standard character output (often by default a screen)
    // cin      The standard character input (often by default a keyboard)
    // cerr     The standard character error output (unbuffered)
    // clog     The standard character error output (buffered)
    // wcin
    // wcout
    // wcerr
    // wclog

    std::cout << "cout" << std::endl << std::endl;
    std::cerr << std::endl << "cerr" << std::endl;

    /**
     * File Streams
     *
     * In <fstream>, the standard library provides streams to and from a file:
     *      ifstreams for reading from a file
     *      ofstreams for writing to a file
     *      fstreams for reading from and writing to a file
     */

    // Operations

    // fstream fs {};
    // fstream fs {s,m}; // mode m
    // fstream fs {fs2};
    // fs=move(fs2)
    // fs.swap(fs2)
    // p=fs.rdbuf()
    // fs.is_open()
    // fs.open(s,m)
    // fs.close()

    // Stream Modes

    // ios_base::app        Append (i.e., add to the end of the file)
    // ios_base::ate        "At end" (open and seek to the end)
    // ios_base::binary     Binary mode; beware of system-specific behavior
    // ios_base::in         For reading
    // ios_base::out        For writing
    // ios_base::trunc      Truncate the file to 0 length

    // open fail
    std::fstream fs{"not existing"};
    if (!fs) {
        std::cerr << "cannot open file" << std::endl;
    }
    assert(!fs.is_open());

    // open success and write
    fs.open("./io_streams.test", std::ios_base::out);
    assert(fs.is_open());

    std::string content = "content";
    fs << content;

    assert(!fs.bad());

    fs.close();

    // open success and read
    fs.open("./io_streams.test", std::ios_base::in);
    assert(fs.is_open());

    std::string read_content;
    fs.seekp(2);
    fs >> read_content;
    assert(read_content == "ntent");

    assert(!fs.bad());
    assert(!fs.fail());

    fs.close();

    // open success and write fail (try to write to file open for read)
    fs.open("./io_streams.test", std::ios_base::in);
    assert(fs.is_open());

    fs << "text";

    assert(fs.bad());
    assert(fs.fail());

    fs.close();

    /**
     * String Streams
     *
     * In <sstream>, the standard library provides streams to and from a string :
     *      istringstreams for reading from a string
     *      ostringstreams for writing to a string
     *      stringstreams for reading from and writing to a string
     */

    // Operations

    // stringstream ss {m};
    // stringstream ss {};
    // stringstream ss {s,m};
    // stringstream ss {s};
    // stringstream ss {ss2};
    // ss=move(ss2)
    // p=ss.rdbuf()
    // s=ss.str()
    // ss.str(s)
    // ss.swap(ss2)

    std::string str = "string";
    std::stringstream ss{str};
    assert(ss.str() == str);

    ss.str("string2");
    assert(ss.str() == "string2");

    ss.str("");
    assert(ss.str().empty());

    ss << str;
    assert(ss.str() == "string");

    str = "new";
    ss >> str;
    assert(str != "new"); // mode std::ios_base::out required

    std::istringstream is{"input"};
    is.seekg(1);
    is >> str;
    assert(str == "nput");

    std::ostringstream os{};
    os << str;
    assert(os.str() == "nput");

    /**
     * Error Handling
     */

    // Stream States

    // An iostream can be in one of four states, defined in basic_ios from <ios> (§38.4.4):
    //      good()      The previous iostream operations succeeded
    //      eof()       We hit end-of-input ("end-of-file")
    //      fail()      Something unexpected happened (e.g., we looked for a digit and found 'x')
    //      bad()       Something unexpected and serious happened (e.g., disk read error)

    std::ifstream file{"./io_streams.test", std::ios_base::in};
    assert(file.is_open());

    assert(file.good());

    file >> read_content;

    assert(file.eof());
    assert(!file.fail());
    assert(!file.bad());

    std::istringstream istream{"1\n2\n3\na\n4\n"};
    for (int i; istream >> i;) {
        std::cout << i << "\n";
    }

    std::cout << "\n";

    istream.clear();
    istream.seekg(0, std::ios::beg);
    std::string fail_str;
    for (; !istream.eof();) {
        int i;
        if (istream >> i) {
            std::cout << i << "\n";
        } else if (istream.fail()) {
            istream.clear();
            if (istream >> fail_str) {
                std::cout << "fail: " << fail_str << "\n";
            }
        }
    }

    // Exception Control
    // Alternatively, errors can be handled using exceptions:

    // st=ios.exceptions()      st is the iostate of ios
    // ios.exceptions(st)Set    ios's iostate to st

    std::cout << "\n";

    std::ifstream ifstream{};
    ifstream.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    try {
        ifstream.open("no file");
    } catch (std::ios_base::failure &e) {
        std::cout << "failure:" << e.code() << "\n";
    }

    std::cout << "\n";

    std::stringstream iostream{};

    struct Io_guard { // RAII for streams
        std::iostream &s;
        const std::ios::iostate old_e = s.exceptions();

        Io_guard(std::iostream &ss, std::ios_base::iostate e) : s{ss} {
            s.exceptions(s.exceptions() | e);
        }

        ~Io_guard() {
            s.exceptions(std::ios_base::goodbit);
        }
    };

    try {
        Io_guard guard{iostream, std::ios_base::failbit};

        int i{};
        iostream >> i;
    } catch (std::ios_base::failure &e) {
        std::cout << e.code() << "\n";
    }
}
