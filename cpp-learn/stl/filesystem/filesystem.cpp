/*
 Since C++17
 https://en.cppreference.com/w/cpp/17
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

int main()
{
    // Space
    std::filesystem::space_info du = std::filesystem::space("/");

    std::cout << du.capacity / 1024 / 1024 / 1024 << '\n';

    // NumberOfFlags
    std::string filename = "filesystem.test";
    if (std::ofstream file{filename}; !file) {
        throw std::runtime_error{"cannot open file"};
    }
    else {
        file << "content";
    }

    std::cout << std::filesystem::file_size(filename) << '\n';

    // File types
    assert(std::filesystem::is_regular_file(filename));

    // Remove
    auto filename2 = filename + "2";
    if (std::filesystem::exists(filename2)) {
        std::filesystem::remove(filename2);
    }

    // Exists
    assert(!std::filesystem::exists(filename2));

    // Copy
    std::filesystem::copy_file(filename, filename2);
    assert(std::filesystem::exists(filename2));

    // Iterate
    std::filesystem::recursive_directory_iterator it{"."};
    for (const auto &path : it) {
        std::cout << path << '\n';
    }
}
