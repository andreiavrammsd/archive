/*
 The C++ Programming Language, 4th Edition
 */

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string address_cpp(const std::string &identifier, const std::string &domain);

char *address_c(const char *identifier, const char *domain);

static int cmp(const void *a, const void *b);

std::string return_by_value_and_move(const std::string &in);

int main()
{
    /**
     * string vs. C-Style Strings
     */

    // string
    std::string s1 = "Ring";

    assert(s1 == "Ring");
    assert(!(s1 < "Opera"));

    std::string s2 = address_cpp(s1, "Valkyrie");
    assert(s2 == "Ring@Valkyrie");

    std::vector<std::string> vs = {"Grieg", "Williams", "Bach", "Handel"};
    sort(vs.begin(), vs.end());
    assert(vs == (std::vector<std::string>{"Bach", "Grieg", "Handel", "Williams"}));

    // C-Style Strings
    char s3[] = "Ring";

    assert(strcmp(s3, "Ring") == 0);
    assert(strcmp(s3, "Opera") > 0);

    char *s4 = address_c(s3, "Valkyrie");
    assert(strcmp(s4, "Ring@Valkyrie") == 0);
    free(s4);

    const char *as[] = {"Grieg", "Williams", "Bach", "Handel"};
    qsort(as, sizeof(as) / sizeof(*as), sizeof(*as), cmp);
    std::vector<const char *> vv(as, as + 4);
    assert(vv == (std::vector<const char *>{"Bach", "Grieg", "Handel", "Williams"}));

    // C-Style Strings comparison
    // https://lemire.me/blog/2020/04/30/for-case-insensitive-string-comparisons-avoid-char-by-char-functions/
    const size_t sz = 3;
    const char *string1 = "abc";
    const char *string2 = "abc";

    bool isequal = (strncasecmp(string1, string2, sz) == 0);
    assert(isequal);

    isequal = true;
    for (size_t i = 0; i < sz; i++) {
        if (tolower(string1[i]) != tolower(string2[i])) {
            isequal = false;
            break;
        }
    }
    assert(isequal);

    /**
     * Constructors
     */

    auto a = std::allocator<std::basic_string<char>>();
    std::basic_string<char> bs1{a};

    std::basic_string<char> bs2;

    std::basic_string<char> bs3{bs2, a};

    std::basic_string<char> bs4{bs2};

    // basic_string s {s2,pos,n,a};
    // basic_string s {s2,pos,n};
    // basic_string s {s2,pos};
    // basic_string s {p,n,a};
    // basic_string s {p,n};
    // basic_string s {n,c,a};
    // basic_string s {n,c};
    // basic_string s {b,e,a};
    // basic_string s {b,e};

    auto bs5 = bs4;

    auto bs6 = move(bs5);

    // string is alias for basic_string<char>

    std::string str0;                        // the empty string
    std::string str1{"As simple as that!"};  // construct from C-style string
    std::string str2{s1};                    // copy constructor

    std::string str6{0};  // danger: passing nullptr
    const char *p = 0;    // set p to "no string"
    assert(p == nullptr);

    // Unfortunately, the compiler cannot catch the definition of s6
    // or the even nastier case of a const char* holding the nullptr:
    // string s6 {0}; // danger: passing nullptr
    // string s7 {p}; // may or may not be OK depending on the value of p
    // string s8 {"OK"}; // OK: pass pointer to C-style string

    /**
     * Fundamental Operations
     */

    std::string str{"ABC"};

    // Comparisons

    assert(str == std::string{"ABC"});
    assert(str == "ABC");
    // s!=s2
    // s<s2
    // s<=s2
    // s>s2
    // s>=s2

    // NumberOfFlags and Capacity

    assert(str.size() == 3);
    // n=s.length()
    // n=s.max_size()
    // s.resize(n,c)
    // s.resize(n)
    // s.reserve(n)
    // s.reserve()
    // n=s.capacity()
    // s.shrink_to_fit()
    // s.clear()
    assert(!str.empty());
    // a=s.get_allocator()

    // Access

    assert(str[1] == 'B');
    assert(str.at(0) == 'A');
    // s.front()
    assert(str.back() == 'C');
    // s.push_back(c)
    // s.pop_back()
    // s+=x
    // s=s1+s2
    // n2=s.copy(s2,n,pos)
    // n2=s.copy(s2,n)
    // p=s.c_str()
    // p=s.data()
    // s.swap(s2)
    // swap(s,s2)

    /**
     * String I/O
     */

    std::stringstream ss;
    ss << "DEF";
    ss >> str;
    assert(str == "DEF");

    std::stringstream ss2;
    ss2 << str;
    assert(ss2.str() == "DEF");

    std::stringstream in;
    in << "A\nB\nC\n";

    getline(in, str);
    assert(str == "A");
    getline(in, str);
    assert(str == "B");

    in.str("A,B,C,");
    getline(in, str, ',');
    assert(str == "A");
    getline(in, str, ',');
    assert(str == "B");

    in.str("A\nB\nC");
    std::vector<std::string> lines;
    for (std::string s; getline(in, s);) {
        lines.push_back(s);
    }
    assert(lines == (std::vector<std::string>{"A", "B", "C"}));

    // std::string sa;
    // std::string sb;
    // std::cin >> sa >> sb;
    // std::cout << sa << sb << "\n";

    /**
     * Numeric Conversions
     */

    std::string num = "123";

    // x=stoi(s,p,b)
    // x=stoi(s,p)
    assert(stoi(num) == 123);
    assert(stoi(num) == 123);
    assert(typeid(stoi(num)) == typeid(int));
    assert(stoi(std::string{"12a3"}) == 12);

    try {
        stoi(std::string{"_123"});
        assert(false);
    }
    catch (std::invalid_argument &) {
        static_assert(true);
    }

    num = "ffffff";
    assert(stoi(num, nullptr, 16) == 16777215);

    // x=stol(s,p,b)
    // x=stoul(s,p,b)
    // x=stoll(s,p,b)
    // x=stoull(s,p,b)
    // x=stof(s,p)
    // x=stod(s,p)
    // x=stold(s,p)
    assert(std::to_string(34) == std::string{"34"});
    // ws=to_wstring(x)

    /**
     * STL-like Operations
     */

    // String Iterators

    str = "AbC";

    assert(*str.begin() == 'A');
    // p=s.end()
    // p=s.cbegin()
    // p=s.cend()
    // p=s.rbegin()
    // p=s.rend()
    // p=s.crbegin()
    // p=s.crend()

    auto cp = find_if(str.cbegin(), str.cend(), islower);
    assert(cp == str.cbegin() + 1);
    assert(*cp == 'b');

    // Assignment

    // s.assign(x)
    // s.assign(move(s2))
    // s.assign(s2,pos,n)
    // s.assign(p,n)
    // s.assign(n,c)
    // s.assign(b,e)

    // Insertion and Deletion

    str.append("d");
    assert(str == "AbCd");
    // s.append(b,e)
    // s.append(s2,pos,n)
    // s.append(p,n)
    // s.append(n,c)
    str.insert(0, "0");
    assert(str == "0AbCd");
    // s.insert(p,c)
    // s.insert(p,n,c)
    // insert(p,b,e)
    // s.erase(pos)
    str.erase(1, 2);
    assert(str == "0Cd");

    std::string full_name = "Dennis Ritchie";
    std::string middle_name = "MacAlistair";
    full_name.insert(full_name.find(' '), ' ' + middle_name);
    assert(full_name == "Dennis MacAlistair Ritchie");

    // Replace

    // s.replace(pos,n,s2,pos2,n2)
    // s.replace(pos,n,p,n2)
    // s=s.replace(pos,n,s2)
    // s.replace(pos,n,n2,c)
    // s.replace(b,e,x)
    // s.replace(b,e,p,n)
    str.replace(str.begin(), str.begin() + 2, 2, '*');
    assert(str == "**d");
    // s.replace(b,e,b2,e2)

    str = "but I have heard it works even if you don't believe in it";
    str.replace(0, 4, "");  // erase initial "but"
    str.replace(str.find("even"), 4, "only");
    str.replace(str.find(" don't"), 6, "");  // erase by replacing with ""
    assert(str == "I have heard it works only if you believe in it");

    // A replace() returns a reference to the object for which it was called.
    // This can be used for chaining operations:
    str = "but I have heard it works even if you don't believe in it";
    str.replace(0, 4, "").replace(str.find("even"), 4, "only").replace(str.find(" don't"), 6, "");
    assert(str == "I have heard it works only if you believe in it");

    str = "but I have heard it works even if you don't believe in it";
    try {
        str.replace(str.find("NOT IN STRING"), 4, "only");
        assert(false);
    }
    catch (std::out_of_range &) {
        static_assert(true);
    }

    try {
        str.resize(str.max_size() + 1);
        assert(false);
    }
    catch (std::length_error &) {
        static_assert(true);
    }

    /**
     * The find Family
     */

    // Find Element

    str = "ABC";

    // pos=s.find(x)
    assert(str.find('B', 2) == std::string::npos);
    // pos=s.find(p,pos2,n)
    // pos=s.rfind(x,pos2)
    assert(str.rfind('C') == 2);
    assert(str.rfind('E') == std::string::npos);
    // pos=s.rfind(p,pos2,n)

    // Find Elements from a Set

    // pos2=s.find_first_of(x,pos)
    // pos=s.find_first_of(x)
    // pos2=s.find_first_of(p,pos,n)
    // pos2=s.find_last_of(x,pos)
    // pos=s.find_last_of(x)
    // pos2=s.find_last_of(p,pos,n)
    // pos2=s.find_first_not_of(x,pos)
    // pos=s.find_first_not_of(x)
    // pos2=s.find_first_not_of(p,pos,n)
    assert(str.find_last_not_of('C', 1) == 1);
    // pos=s.find_last_not_of(x)
    // pos2=s.find_last_not_of(p,pos,n)

    /**
     * Substrings
     */

    str = "ABCDEF";

    // Substrings

    auto substr = str.substr(0, 3);
    assert(substr == "ABC");

    substr = str.substr(3);
    assert(substr == "DEF");

    substr = str.substr();
    assert(substr == "ABCDEF");

    // Compare

    assert(str.compare(std::string{"CDE"}) < 0);
    assert(str.compare("ABCDEF") == 0);
    // n2=s.compare(pos,n,s2)
    // n2=s.compare(pos,n,s2,pos2,n2)
    // n=s.compare(p)
    // n2=s.compare(pos,n,p)
    const char *cstr = "ABCDEF";
    assert(str.compare(0, 3, cstr, 3) == 0);

    /**
     * Return by value and move
     */

    assert(return_by_value_and_move("in") == "inout");
}

std::string address_cpp(const std::string &identifier, const std::string &domain) { return identifier + '@' + domain; }

char *address_c(const char *identifier, const char *domain)
{
    int iden_len = strlen(identifier);
    int dom_len = strlen(domain);
    char *addr = (char *)malloc(iden_len + dom_len + 2);  // remember space for 0 and '@'
    strcpy(addr, identifier);
    addr[iden_len] = '@';
    strcpy(addr + iden_len + 1, domain);
    return addr;
}

static int cmp(const void *a, const void *b) { return strcmp(*(const char **)a, *(const char **)b); }

std::string return_by_value_and_move(const std::string &in)
{
    std::string out = in + "out";
    return out;
}