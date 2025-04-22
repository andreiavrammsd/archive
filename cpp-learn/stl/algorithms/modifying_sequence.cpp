/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <random>
#include <sstream>
#include <vector>

/**
 * copy
 * copy_if
 * copy_n
 * copy_backward
 * move
 * move_backward
 * fill
 * fill_n
 * transform
 * generate
 * generate_n
 * remove
 * remove_if
 * remove_copy
 * remove_copy_if
 * replace
 * replace_if
 * replace_copy
 * replace_copy_if
 * swap
 * swap_ranges
 * iter_swap
 * reverse
 * reverse_copy
 * rotate
 * rotate_copy
 * shift_left
 * shift_right
 * random_shuffle
 * shuffle
 * sample
 * unique
 * unique_copy
 */

template <typename T>
std::vector<T> expected(std::initializer_list<T>);

template <class In, class Out, class Op>
Out transform_impl(In first, In last, Out res, Op op);

void touppers(std::string &);

int main()
{
    std::vector<int> nums = {1, 2, 3, 3};
    std::vector<int> out;

    // copy
    copy(nums.begin() + 1, nums.end(), std::back_inserter(out));
    assert(out == expected({2, 3, 3}));

    // The target of a copy algorithm need not be a container. Anything that can be described by an
    // output iterator (§38.5) will do. For example:
    std::list<int> list = {1, 2, 3};
    std::stringstream str;
    copy(list.begin(), list.end(), std::ostream_iterator<int>(str));
    assert(str.str() == "123");

    // copy_if (C++11)
    copy_if(nums.begin(), nums.end(), out.begin(), [](int x) { return x % 2 != 0; });
    assert(out == expected({1, 3, 3}));

    // copy_n (C++11)
    copy_n(nums.begin(), 2, out.begin());
    assert(out == expected({1, 2, 3}));

    // copy_backward
    out.resize(out.size() * 2 + 1);
    copy_backward(nums.begin(), nums.end(), out.end());
    assert(out == expected({1, 2, 3, 1, 2, 3, 3}));
    out.resize(4);
    assert(out == expected({1, 2, 3, 1}));

    // move (C++11)
    std::vector<std::string> words{"the", "quick", "brown", "fox"};
    std::vector<std::string> words_out(words.size());

    move(words.begin(), words.end(), words_out.begin());

    std::stringstream ws;

    copy(words.begin(), words.end(), std::ostream_iterator<std::string>(ws));
    assert(ws.str().empty());

    ws.str("");
    copy(words_out.begin(), words_out.end(), std::ostream_iterator<std::string>(ws));
    assert(ws.str().length() == 16);

    // move_backward (C++11)
    nums[0] = 9;
    move_backward(nums.begin(), nums.end(), out.begin() + 5);
    nums[0] = 1;
    assert(out == expected({1, 9, 2, 3}));

    // fill
    nums.clear();
    nums.resize(3);

    fill(nums.begin(), nums.end(), 3);
    assert(nums == expected({3, 3, 3}));

    // fill_n
    fill_n(nums.begin(), 2, 2);
    assert(nums == expected({2, 2, 3}));

    // transform
    std::transform(nums.begin(), nums.end(), std::back_inserter(out), [](int x) { return 2 * x; });
    assert(out == expected({1, 9, 2, 3, 4, 4, 6}));

    std::vector<int> xs = {1, 2, 3, 3};
    std::vector<int> ys = {0, 3, 1, 0};
    std::vector<int> zs;
    std::transform(xs.begin(), xs.end(), ys.begin(), std::back_inserter(zs), [](int x, int y) { return x + y; });
    assert(zs == expected({1, 5, 4, 3}));

    std::vector<bool> t = {false, true, true};
    std::vector<bool> f = {false, true, false};
    std::vector<bool> r;
    std::transform(t.begin(), t.end(), f.begin(), std::back_inserter(r), std::logical_and<>{});
    assert(r == expected({false, true, false}));

    std::string text = "abc";
    touppers(text);
    assert(text == "ABC");

    std::string low = "ABC";
    auto iter = std::transform(low.cbegin(), low.cend() - 1, low.begin(), ::tolower);
    assert(low == "abC");
    assert(iter == low.cend() - 1);

    // generate
    generate(nums.begin() + 1, nums.end(), []() { return 22; });
    assert(nums == expected({2, 22, 22}));

    // generate_n
    auto ptr = generate_n(nums.begin() + 1, 1, []() { return 33; });
    assert(nums == expected({2, 33, 22}));
    assert(*ptr == 22);

    generate_n(std::ostream_iterator<unsigned short>{std::cout, ", "}, 3, std::mt19937(std::random_device()()));
    std::cout << '\n';

    // remove
    nums = {1, 2, 1, 3, 4, 5, 6};

    auto p = remove(nums.begin(), nums.end(), 9);
    assert(p == nums.end());
    assert(nums == expected({1, 2, 1, 3, 4, 5, 6}));

    p = remove(nums.begin(), nums.end(), 1);
    assert(p == nums.end() - 2);
    assert(nums == expected({2, 3, 4, 5, 6, 5, 6}));

    // remove_if
    nums = {1, 2, 1, 3, 4, 5, 6};
    remove_if(nums.begin(), nums.end(), [](int q) { return q % 2 == 0; });
    assert(nums == expected({1, 1, 3, 5, 4, 5, 6}));

    std::vector<std::string> paths = {
        "/user/algo2.cpp",
        "/user/main.cpp",
        "/user/algo3.cpp",
    };
    assert(paths.size() == 3);
    std::vector<std::string> algo_paths;
    auto removed = std::remove_if(paths.begin(), paths.end(),
                                  [](std::string path) { return path.find("algo") == std::string::npos; });
    assert(removed != paths.end());
    paths.erase(removed);
    assert(paths.size() == 2);
    assert(paths == expected<std::string>({"/user/algo2.cpp", "/user/algo3.cpp"}));

    // remove_copy_if
    nums = {1, 2, 1, 3, 4, 5, 6};
    out.clear();
    remove_copy(nums.begin(), nums.end(), std::back_inserter(out), 1);
    assert(out == expected({2, 3, 4, 5, 6}));

    // These algorithms cannot change the size of their input sequence, so even remove() leaves the size of
    // its input sequence unchanged. Like unique(), it "removes" by moving elements to the left.
    std::string s{"*CamelCase*IsUgly*"};
    std::stringstream ex;

    auto it = remove(s.begin(), s.end(), '*');
    copy(s.begin(), it, std::ostream_iterator<char>{ex});

    assert(ex.str() == "CamelCaseIsUgly");
    assert(s == "CamelCaseIsUglyly*");

    // replace
    nums = {1, 2, 3, 2};
    replace(nums.begin(), nums.end(), 2, 3);
    assert(nums == expected({1, 3, 3, 3}));

    // replace_if
    nums = {1, 2, 3, 2};
    replace_if(nums.begin(), nums.end(), [](int i) { return i % 2 == 0; }, -1);
    assert(nums == expected({1, -1, 3, -1}));

    // replace_copy
    nums = {1, 2, 3, 2};
    out.clear();
    replace_copy(nums.begin(), nums.end(), std::back_inserter(out), 2, 9);
    assert(out == expected({1, 9, 3, 9}));

    // replace_copy_if
    nums = {1, 2, 3, 2};
    out.clear();
    replace_copy_if(nums.begin(), nums.end(), std::back_inserter(out), [](int x) { return x % 2 == 0; }, 9);
    assert(out == expected({1, 9, 3, 9}));

    // swap
    std::vector<int> x = {2, 1};
    std::vector<int> y = {3, 4};

    swap(x, y);
    assert(x == expected({3, 4}));
    assert(y == expected({2, 1}));

    // swap_ranges
    x = {1, 9, 3, 4};
    y = {5, 6, 7};

    // if not the same size, there will be a leak
    x.resize(y.size());
    y.resize(x.size());

    swap_ranges(x.begin(), x.end(), y.begin());
    assert(x == expected({5, 6, 7}));
    assert(y == expected({1, 9, 3}));

    // iter_swap
    x = {1, 9, 3, 4};

    iter_swap(x.begin(), x.end() - 1);
    assert(x == expected({4, 9, 3, 1}));

    // reverse
    nums = {1, 2, 1, 3, 4, 5, 6};
    reverse(nums.begin(), nums.end());
    assert(nums == expected({6, 5, 4, 3, 1, 2, 1}));

    // reverse_copy
    nums = {1, 2, 1, 3, 4, 5, 6};
    out.clear();
    reverse_copy(nums.begin(), nums.end(), std::back_inserter(out));
    assert(out == expected({6, 5, 4, 3, 1, 2, 1}));

    // rotate
    nums = {1, 2, 3, 4, 5};
    rotate(nums.begin(), nums.begin() + 2, nums.end());
    assert(nums == expected({3, 4, 5, 1, 2}));

    // rotate_copy
    nums = {1, 2, 3, 4, 5};
    std::vector<int> rotated_nums;
    auto pivot = std::find(nums.begin(), nums.end(), 3);
    rotate_copy(nums.begin(), pivot, nums.end(), std::back_inserter(rotated_nums));
    assert(rotated_nums == expected({3, 4, 5, 1, 2}));

    // shuffle (C++11)
    nums = {1, 2, 3, 4, 5};
    shuffle(nums.begin(), nums.end(), std::mt19937(std::random_device()()));
    assert(nums != expected({1, 2, 3, 4, 5}));  // can equal to

    // sample (C++17)
    std::string instr = "abcdefg", outstr;
    std::sample(instr.begin(), instr.end(), std::back_inserter(outstr), 3, std::mt19937{std::random_device{}()});
    std::cout << outstr << '\n';

    // unique
    nums = {1, 2, 2, 3, 4, 4, 9, 10};
    p = unique(nums.begin(), nums.end());
    assert(nums == expected({1, 2, 3, 4, 9, 10, 9, 10}));
    assert(p == nums.begin() + 6);

    nums = {1, 2, 2, 3, 4, 4, 9, 10};
    p = unique(nums.begin(), nums.end(), [](int x, int y) { return y % x != 0; });
    assert(nums == expected({1, 2, 2, 4, 4, 4, 9, 10}));
    assert(p == nums.begin() + 5);

    // unique_copy
    out.clear();
    nums = {1, 2, 2, 3, 4, 4, 9, 10, 2};
    unique_copy(nums.begin(), nums.end(), std::back_inserter(out));
    assert(out == expected({1, 2, 3, 4, 9, 10, 2}));

    out.clear();
    unique_copy(nums.begin(), nums.end(), std::back_inserter(out), [](int x, int y) { return y % x != 0; });
    assert(out == expected({1, 2, 2, 4, 4}));

    // Like other standard algorithms, unique() operates on iterators. It does not know which container
    // these iterators point into, so it cannot modify that container. It can only modify the values_ of the
    // elements. This implies that unique() does not eliminate duplicates from its input sequence in the
    // way we naively might expect.
    list = {1, 2, 3, 2, 3};

    out.clear();
    unique_copy(list.begin(), list.end(), std::back_inserter(out));
    assert(out == expected({1, 2, 3, 2, 3}));

    list.sort();  // items must be sorted

    out.clear();
    unique_copy(list.begin(), list.end(), std::back_inserter(out));
    assert(out == expected({1, 2, 3}));
}

template <typename T>
std::vector<T> expected(std::initializer_list<T> l)
{
    return std::vector<T>{l};
}

template <class In, class Out, class Op>
Out transform_impl(In first, In last, Out res, Op op)
{
    while (first != last) *res++ = op(*first++);
    return res;
}

void touppers(std::string &s) { transform_impl(s.begin(), s.end(), s.begin(), toupper); }
