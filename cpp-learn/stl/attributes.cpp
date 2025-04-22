/*
 https://www.geeksforgeeks.org/attributes-in-c/

 https://docs.microsoft.com/en-us/cpp/cpp/attributes?view=vs-2019

 A compiler can use this information to generate informational messages, or to apply special logic
 when compiling the attributed code.

 The compiler ignores any attributes that it does not recognize, which means that you cannot define
 your own custom attributes using this syntax.
 */

int main() {
    /**
     * C++11
     */

    // [[noreturn]]
    // [[carries_dependency]]

    /**
     * C++14
     */

    // [[deprecated]] int f();
    // [[deprecated("g() is thread-unsafe. Use h() instead")]]

    /**
     * C++17
     */

    // [[nodiscard]]
    // [[fallthrough]]
    // [[maybe_unused]]
}
