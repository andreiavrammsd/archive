/*
 The C++ Programming Language, 4th Edition
 */

#include <map>
#include <string>
#include <cassert>

class Std_interface {
   public:
    virtual std::string start() = 0;

    virtual std::string suspend() = 0;

    virtual ~Std_interface() = default;
};

using Pstd_mem = std::string (Std_interface::*)();  // pointer-to-member type

std::map<std::string, Std_interface *> variable;
std::map<std::string, Pstd_mem> operation;

std::string call_member(const std::string &var, const std::string &oper) { return (variable[var]->*operation[oper])(); }

std::string f(Std_interface *p) { return p->start(); }

class First : public Std_interface {
   public:
    std::string start() override { return "first start"; }

    std::string suspend() override { return "first suspend"; }
};

class Second : public Std_interface {
   public:
    std::string start() override { return "second start"; }

    std::string suspend() override { return "second suspend"; }
};

int main()
{
    variable["first"] = new First;
    variable["second"] = new Second;
    operation["suspend"] = &Std_interface::suspend;

    assert(call_member("first", "suspend") == "first suspend");
    assert(call_member("second", "suspend") == "second suspend");

    assert(f(new First) == "first start");
    assert(f(new Second) == "second start");
}
