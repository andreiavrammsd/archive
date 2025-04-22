/*
 The C++ Programming Language, 4th Edition
 */

#include <locale> // Represent cultural differences
#include <clocale> // Represent cultural differences C-style
#include <codecvt> // Code conversion facets

#include <iostream>

int main() {
    // Represent cultural differences
    std::locale previous_locale;
    try {
        previous_locale = std::locale::global(std::locale{""});
        std::cout << previous_locale.name() << "\n\n";
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << "\n\n";
    }

    // Represent cultural differences C-style

    // Code conversion facets
}
