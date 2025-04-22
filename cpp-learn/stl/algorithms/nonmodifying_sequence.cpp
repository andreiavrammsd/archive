/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>

#include <vector>
#include <sstream>
#include <cassert>
#include <functional>

/**
 * all_of
 * any_of
 * none_of
 * for_each
 * for_each_n
 * count
 * count_if
 * mismatch
 * find
 * find_if
 * find_if_not
 * find_end
 * find_first_of
 * adjacent_find
 * search
 * search_n
 */

std::vector<double> scale(const std::vector<double> &, const std::vector<double> &);

template<class In, class In2, class Pred = std::equal_to<typename In::value_type>>
std::pair<In, In2> mismatch_impl(In first, In last, In2 first2, Pred p = {});

bool in_quote(const std::string &s, const std::string &quote);

template<class Container, class Size, class T>
bool consecutive_values(const Container &c, Size count, const T &v);

int main() {
    std::vector<int> nums = {1, 2, 3, 3};
    std::vector<int> nums2 = {3, 5, 2};
    std::vector<int> nums3 = {1, 2, 3, 3, 4};

    // all_of (C++11)
    auto lt0 = all_of(nums.begin(), nums.end(), [](int x) { return x < 0; });
    assert(!lt0);

    std::vector<double> val = {4, 12, 8};
    std::vector<double> div = {2, 4, 8, 1};
    std::vector<double> expected = {2, 3, 1};
    assert(scale(val, div) == expected);

    // any_of (C++11)
    auto even = any_of(nums.begin(), nums.end(), [](int x) { return x % 2 == 0; });
    assert(even);

    // none_of (C++11)
    auto gt10 = none_of(nums.begin(), nums.end(), [](int x) { return x > 10; });
    assert(gt10);

    // for_each
    std::stringstream str;
    for_each(nums.begin(), nums.end(), [&str](int x) { str << x; });
    assert(str.str() == "1233");

    // for_each_n (C++17)
    str.str("");
    str.clear();
    for_each_n(nums.begin(), 2, [&str](int x) { str << x; });
    assert(str.str() == "12");

    // count
    auto c1 = count(nums.begin(), nums.end(), 3);
    assert(c1 == 2);

    // count_if
    c1 = count_if(nums.begin(), nums.end(), [](int x) { return x == 3; });
    assert(c1 == 2);

    std::string sentence = "the quick brown fox\t";
    auto n_whitespace = count_if(sentence.begin(), sentence.end(), isspace);
    assert(n_whitespace == 4);

    // mismatch
    auto mm = mismatch(nums.begin(), nums.end(), nums2.begin());
    assert(*mm.first == 1);
    assert(*mm.second == 3);

    mm = mismatch_impl(nums.begin(), nums.end(), nums2.begin());
    assert(*mm.first == 1);
    assert(*mm.second == 3);

    mm = mismatch(nums.begin(), nums.end(), nums2.begin(), [](int x, int) { return x - 1 == 0; });
    assert(*mm.first == 2);
    assert(*mm.second == 5);

    // find
    auto p = find(nums.begin(), nums.end(), 3);
    assert(*p == 3);
    assert(p == nums.begin() + 2);
    assert(p != nums.begin() + 3);

    p = find(nums.begin(), nums.end(), 6);
    assert(p == nums.end());
    assert(*p == 0);

    // find_if
    p = find_if(nums.begin(), nums.end(), [](int x) { return x > 2; });
    assert(*p == 3);
    assert(p == nums.begin() + 2);
    assert(p != nums.begin() + 3);

    // find_if_not (C++11)
    p = find_if_not(nums.begin(), nums.end(), [](int x) { return x % 2 != 0; });
    assert(*p == 2);

    // find_end
    p = find_end(nums.begin(), nums.end(), nums2.begin(), nums2.end());
    assert(*p == 0);
    assert(p == nums.end());

    p = find_end(nums.begin(), nums.end(), nums2.begin(), nums2.end(), [](int x, int y) { return x - y != 0; });
    assert(*p == 2);

    // find_first_of
    p = find_first_of(nums.begin(), nums.end(), nums2.begin(), nums2.end());
    assert(*p == 2);

    p = find_first_of(nums.begin(), nums.end(), nums2.begin(), nums2.end(),
                      [](int x, int y) { return x - y == 0; });
    assert(*p == 2);

    // adjacent_find
    p = adjacent_find(nums.begin(), nums.end());
    assert(*p == 3);

    p = adjacent_find(nums.begin(), nums.end(), [](int x, int y) { return x - y != 0; });
    assert(*p == 1);

    // search (C++17)
    // default_searcher
    // boyer_moore_searcher
    // boyer_moore_horspool_searcher
    std::vector<int> needle = {2, 3};
    auto it = std::search(nums.cbegin(), nums.cend(), std::default_searcher{needle.cbegin(), needle.cend()});
    assert(it != nums.cend());
    assert((it - nums.begin()) == 1);

    std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit,"
                       " sed do eiusmod tempor incididunt ut labore et dolore magna aliqua";
    std::string search = "pisci";
    auto sit = std::search(text.begin(), text.end(),
                           std::default_searcher(
                                   search.begin(), search.end()));
    assert(sit != text.end());
    assert(sit - text.begin() == 43);

    std::string quote{"Why waste time learning, when ignorance is instantaneous?"};
    assert(in_quote("learning", quote));
    assert(!in_quote("lemming", quote));
    // [...] search() is a useful algorithm for finding a substring generalized to all sequences.
    // Use find() or binary_search() (§32.6) to look for just a single element.

    // search_n (C++17)
    const char sequence[] = "1001010100010101001010101";
    assert(!consecutive_values(sequence, 4, '0'));
    assert(consecutive_values(sequence, 3, '0'));
}

std::vector<double> scale(const std::vector<double> &val, const std::vector<double> &div) {
    assert(val.size() < div.size());
    assert(all_of(div.begin(), div.end(), [](double x) { return 0 < x; }));

    std::vector<double> res(val.size());
    for (size_t i = 0; i < val.size(); i++) {
        res[i] = val[i] / div[i];
    }
    return res;
}

template<class In, class In2, class Pred>
std::pair<In, In2> mismatch_impl(In first, In last, In2 first2, Pred p) {
    while (first != last && p(*first, *first2)) {
        ++first;
        ++first2;
    }
    return {first, first2};
}

bool in_quote(const std::string &s, const std::string &quote) {
    auto p = search(quote.begin(), quote.end(), s.begin(), s.end()); // find s in quote
    return p != quote.end();
}

template<class Container, class Size, class T>
bool consecutive_values(const Container &c, Size count, const T &v) {
    return std::search_n(std::begin(c), std::end(c), count, v) != std::end(c);
}