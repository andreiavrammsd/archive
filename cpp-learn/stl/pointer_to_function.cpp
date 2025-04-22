/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <iostream>

void error(const std::string &s);

void (*efct)(const std::string &s);

using Type = void(const std::string &s);  // Alias to function type

void call(Type f);

int main()
{
    efct = &error;
    efct("error to print");

    call(&error);
}

void error(const std::string &s) { std::cerr << s << std::endl; }

void call(Type f) { f("second error"); }
