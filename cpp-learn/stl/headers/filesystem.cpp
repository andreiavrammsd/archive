/*
 https://en.cppreference.com/w/cpp/17
 */

// C++17
#include <filesystem> // Filesystem support

#include <cassert>

int main() {
    // Filesystem support
    std::string dir = "filesystem-test-dir-cpp17";
    std::string dir_other = dir + "-other";

    if (std::filesystem::exists(dir)) {
        std::filesystem::remove(dir);
    }
    if (std::filesystem::exists(dir_other)) {
        std::filesystem::remove(dir_other);
    }

    assert(std::filesystem::create_directory(dir));
    assert(std::filesystem::exists(dir));

    std::filesystem::rename(dir, dir_other);
    assert(!std::filesystem::exists(dir));

    std::filesystem::file_status status = std::filesystem::status(dir_other);
    assert(status.type() == std::filesystem::file_type::directory);
}
