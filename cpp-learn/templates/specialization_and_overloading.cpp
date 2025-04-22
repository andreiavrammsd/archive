/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <climits>
#include <cstring>
#include <iostream>
#include <vector>

template <typename T>
bool less(T a, T b)
{
    return a < b;
}

template <typename T>
void sort(std::vector<T> &v)
{
    const auto n = static_cast<int>(v.size());

    for (int gap = n / 2; 0 < gap; gap /= 2) {
        for (int i = gap; i != n; ++i) {
            for (int j = i - gap; 0 <= j; j -= gap) {
                if (less(v[j + gap], v[j])) {
                    std::swap(v[j], v[j + gap]);
                }
            }
        }
    }
}

template <>
// bool less<const char *>(const char *a, const char *b) {
// [...] the template argument can be deduced from the function argument list [...]
// bool less<>(const char *a, const char *b) {
// Given the template<> prefix, the second empty <> is redundant [...]
bool less(const char *a, const char *b)
{
    return std::strcmp(a, b) < 0;
}

// There are a few uses of function specializations. For example, we can select among
// functions taking no arguments:
template <typename T>
T max_value();  // no definition

template <>
constexpr int max_value<int>()
{
    return INT_MAX;
}

template <>
constexpr char max_value<char>()
{
    return CHAR_MAX;
}

template <typename Iter>
auto my_algo(Iter)
{
    using Value_type = typename Iter::value_type;
    auto x = max_value<Value_type>();
    return x;
}

int main()
{
    std::vector<int> vi{2, 4, 1};
    sort(vi);
    assert(vi == std::vector<int>({1, 2, 4}));

    std::vector<const char *> vc{(char *)"b", (char *)"c", (char *)"a"};
    sort(vc);
    assert(vc == std::vector<const char *>({(char *)"a", (char *)"b", (char *)"c"}));

    assert(my_algo(std::vector<int>{1}) == INT_MAX);
    assert(my_algo(std::vector<char>{'1'}) == CHAR_MAX);
}
