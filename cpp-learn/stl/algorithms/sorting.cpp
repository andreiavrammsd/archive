/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>

#include <vector>
#include <cassert>
#include <string>
#include <sstream>
#include <iterator>
#include <list>
#include <cstring>

/**
 * is_sorted
 * is_sorted_until
 * sort
 * partial_sort
 * partial_sort_copy
 * stable_sort
 * nth_element
 */

template<typename T>
std::vector<T> expected(std::initializer_list<T>);

struct Employee {
    int age;
    std::string name;
};

bool operator<(const Employee &a, const Employee &b);

struct Book {
    std::string title;
    int copies_sold;
};

std::ostream &operator<<(std::ostream &os, const Book &book);

std::vector<Book> bestsellers(const std::vector<Book> &sales, int count);

int main() {
    std::vector<int> nums;
    std::vector<int> out;

    // is_sorted (C++11)
    nums = {9, 1, 3, 4};

    auto sorted = is_sorted(nums.begin(), nums.end());
    assert(!sorted);

    sorted = is_sorted(nums.begin() + 1, nums.end());
    assert(sorted);

    // is_sorted_until (C++11)
    nums = {1, 3, 9, 4};
    auto p = is_sorted_until(nums.begin(), nums.end());
    assert(p == nums.begin() + 3);
    assert(*p == 4);

    nums = {1, 3, 9, 10};
    p = is_sorted_until(nums.begin(), nums.end());
    assert(p == nums.end());

    // sort
    nums = {9, 1, 4, 3};
    sort(nums.begin(), nums.begin() + 2);
    assert(nums == expected({1, 9, 4, 3}));

    nums = {9, 1, 4, 3};
    sort(nums.begin(), nums.end(), std::greater<>{});
    assert(nums == expected({9, 4, 3, 1}));

    // partial_sort
    nums = {9, 1, 4, 3};
    partial_sort(nums.begin(), nums.begin() + 2, nums.end());
    assert(nums == expected({1, 3, 9, 4}));

    // partial_sort_copy
    out.clear();
    out.resize(4);

    nums = {9, 1, 4, 3};
    partial_sort_copy(nums.begin(), nums.end(), out.begin(), out.end());
    assert(out == expected({1, 3, 4, 9}));
    assert(nums == expected({9, 1, 4, 3}));

    // stable_sort
    std::vector<Employee> employees{
            {108, "Zaphod"},
            {32,  "Arthur"},
            {108, "Ford"},
    };

    stable_sort(employees.begin(), employees.end());
    assert(employees[0].name == "Arthur");
    assert(employees[1].name == "Zaphod");
    assert(employees[2].name == "Ford");

    // nth_element
    nums = {1, 3, 9, 4, 5};
    nth_element(nums.begin(), nums.begin() + 2, nums.end());
    assert(nums == expected({3, 1, 4, 5, 9}));

    // The standard list (§31.3) does not provide random-access iterators, so lists should be sorted
    // using the specific list operations (§31.4.2) or by copying their elements into a vector, sorting that
    // vector, and then copying the elements back into the list:
    std::list<int> list = {1, 7, 3};

    std::vector<int> vlist = {list.begin(), list.end()};
    sort(vlist.begin(), vlist.end());
    copy(vlist.begin(), vlist.end(), list.begin());
    // list.sort();

    std::list<int> listcmp = {1, 3, 7};
    assert(list == listcmp);

    // Sometimes, only the first elements of a sorted sequence are needed. In that case, it makes sense
    // to sort the sequence only as far as is needed to get the first part in order, that is, a partial sort. The
    // plain partial_sort(b,m,e) algorithms put the elements in the range [b:m) in order. The partial_sort_copy()
    // algorithms produce N elements, where N is the lower of the number of elements in
    // the output sequence and the number of elements in the input sequence. We need to specify both the
    // start and the end of the result sequence because that's what determines how many elements we need
    // to sort.
    std::vector<Book> sales = {
            {"Abc",           2},
            {"Title",         5},
            {"Title2",        10},
            {"Title3",        3},
            {"Another title", 16},
    };

    auto top_books = bestsellers(sales, 3);

    std::ostringstream top_books_str;
    copy(top_books.begin(), top_books.end(), std::ostream_iterator<Book>{top_books_str, ", "});
    assert(top_books_str.str() == "Another title(16), Title2(10), Title(5), ");

    // If the number of elements desired to be sorted by a partial_sort() is small compared to the total
    // number of elements, these algorithms can be significantly faster than a complete sort(). Then, their
    // complexity approaches O(N) compared to sort()'s O(N*log(N)) .

    // Sorting C-style strings requires an explicit sorting criterion. The reason is that C-style strings
    // are simply pointers with a set of conventions for their use, so < on pointers compares machine
    // addresses rather than character sequences.

    std::vector<std::string> vs = {"Helsinki", "Copenhagen", "Oslo", "Stockholm"};

    sort(vs.begin(), vs.end());
    assert(vs == expected<std::string>({"Copenhagen", "Helsinki", "Oslo", "Stockholm"}));

    std::vector<const char *> vcs = {"Helsinki", "Copenhagen", "Oslo", "Stockholm"};

    sort(vcs.begin(), vcs.end());
    assert(vcs != expected<const char *>({"Copenhagen", "Helsinki", "Oslo", "Stockholm"}));

    sort(vcs.begin(), vcs.end(), [](const char *p, const char *q) { return strcmp(p, q) < 0; });
    assert(vcs == expected<const char *>({"Copenhagen", "Helsinki", "Oslo", "Stockholm"}));
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}

bool operator<(const Employee &a, const Employee &b) {
    return a.age < b.age;
}

std::ostream &operator<<(std::ostream &os, const Book &book) {
    return os << book.title << '(' << book.copies_sold << ')';
}

std::vector<Book> bestsellers(const std::vector<Book> &sales, int count) {
    std::vector<Book> bestsellers(count);

    partial_sort_copy(sales.begin(), sales.end(),
                      bestsellers.begin(), bestsellers.end(),
                      [](const Book &b1, const Book &b2) { return b1.copies_sold > b2.copies_sold; });

    return bestsellers;
}