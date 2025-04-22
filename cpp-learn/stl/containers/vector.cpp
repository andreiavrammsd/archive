/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

std::vector<int> task(std::vector<int> &&v)
{
    std::vector<int> res(v.size());

    std::transform(v.begin(), v.end(), res.begin(), [](int i) { return i * 2; });

    return res;
}

std::vector<int> user(std::vector<int> &large)
{
    std::vector<int> res;
    res = task(std::move(large));  // transfer ownership of data to task()
    return res;
}

template <typename T, size_t M, size_t N>
class Matrix {
   public:
    template <typename... D>
    explicit Matrix(D... d) : elems{d...}
    {
        static_assert(sizeof...(D) == M * N, "");
    }

    T &at(size_t row, size_t col)
    {
        if (row >= M) {
            throw std::range_error{"row"};
        }
        if (col >= N) {
            throw std::range_error{"column"};
        }
        return elems[(row * N) + col];
    }

    void operator()(std::function<T(const T &i)> f);

   private:
    std::vector<T> elems;

    template <typename TT, size_t MM, size_t NN>
    friend bool operator==(const Matrix<TT, MM, NN> &a, const Matrix<TT, MM, NN> &b);
};

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::operator()(std::function<T(const T &i)> f)
{
    for (auto it = elems.begin(); it != elems.end(); ++it) {
        *it = f(*it);
    }
}

template <typename TT, size_t MM, size_t NN>
bool operator==(const Matrix<TT, MM, NN> &a, const Matrix<TT, MM, NN> &b)
{
    auto ita = a.elems.begin();
    auto itb = b.elems.begin();

    while (ita != a.elems.end()) {
        if (*ita != *itb) {
            return false;
        }
        ++ita;
        ++itb;
    }

    return true;
}

// The STL vector is the default container. Use it unless you have a good reason not to.
// If your suggested alternative is a list or a built-in array, think twice.
int main()
{
    std::vector<int> n1{1, 2, 3};

    assert(n1.size() == 3);
    assert(n1.capacity() == 3);
    assert(!n1.empty());

    std::vector<int> n2;

    n2.emplace_back(1);
    n2.push_back(2);
    n2.push_back(3);
    assert(n2.size() == 3);
    assert(n2[2] == 3);

    assert(n1 == n2);

    n1.clear();
    assert(n1.empty());

    std::vector<int> n3{1, 2};
    assert(n3.size() == 2);
    n3.swap(n1);
    assert(n3.empty());
    assert(n1.size() == 2);

    n3.reserve(10);
    assert(n3.capacity() == 10);
    // I used to be careful about using reserve() when I was reading into a vector.
    // I was surprised to find that for essentially all of my uses, calling reserve() did not measurably affect
    // performance. The default growth strategy worked just as well as my estimates, so I stopped trying
    // to improve performance using reserve(). Instead, I use it to increase predictability of reallocation
    // delays and to prevent invalidation of pointers and iterators.
    // The notion of capacity allows for iterators into a vector to be valid unless a reallocation actually
    // happens.

    std::vector<int> n4{n1};
    assert(n4 == n1);

    std::vector<int> n5{std::move(n1)};
    assert(n5 == n4);

    const auto &n6 = n5;
    assert(n6 == n5);

    auto n7 = std::move(n2);
    assert(n7.size() == 3);

    n7.assign(5, 1);
    std::vector<int> n8(5, 1);
    assert(n7 == n8);
    assert(n7.size() == 5);

    assert(n7.capacity() == 5);
    n7.resize(6, 9);
    assert(n7.capacity() == 10);

    std::vector<int> n9{1, 1, 1, 1, 1, 9};
    assert(n7 == n9);
    assert(n7.size() == 6);

    n7.shrink_to_fit();
    assert(n7.size() == 6);
    assert(n7.size() == n7.capacity());

    int vc[] = {1, 2, 3};
    std::vector<int> vcpp(vc, vc + 3);
    assert(vcpp == (std::vector<int>{1, 2, 3}));

    // When changing the size or the capacity, the elements may be moved to new storage locations. That
    // implies that iterators (and pointers and references) to elements may become invalid (i.e., point to
    // the old element locations).

    // Iterators
    auto end = n7.end();  // read/write iterator that points one past the las element in the vector
    n7.erase(end - 1);
    std::vector<int> n10{1, 1, 1, 1, 1};
    assert(n7 == n10);
    assert(n7.capacity() == 6);

    for (auto i : n6) {  // range-for
        std::cout << i << '\n';
    }

    std::vector<int> n11 = {1, 2, 2, 3};
    for (auto p = n11.cbegin(); p != n11.cend(); ++p) {
        if (p != n11.cbegin() && *(p - 1) == *p) {
            std::cout << "duplicate " << *p << '\n';
        }
    }

    // Element Access
    assert(n11.front() == 1);
    assert(n11.back() == 3);
    assert(n11[1] == 2);
    assert(n11.at(2) == 2);

    try {
        n11.at(5);
        assert(false);
    }
    catch (const std::out_of_range &e) {
        std::cout << e.what() << '\n';
    }

    // Containers are often large, so we almost always pass them by reference. However, because they
    // are resource handles (§31.2.1), we can return them (implicitly using move) efficiently. Similarly,
    // we can move them as arguments when we don't want aliasing.
    std::vector<int> numbers = {1, 2, 3};
    std::vector<int> expected = std::vector<int>{2, 4, 6};
    std::vector<int> doubled = user(numbers);
    assert(doubled == expected);

    // Matrix
    Matrix<int, 2, 2> m{1, 2, 3, 4};

    m.at(0, 0) = 9;
    m([](const int &i) { return i * 2; });

    Matrix<int, 2, 2> n{18, 4, 6, 8};
    assert(m == n);

    // Non-member size (C++17)
    assert(std::size(numbers) == 3);

    // Non-member empty (C++17)
    assert(!std::empty(numbers));

    // Non-member data (C++17)
    assert(sizeof(std::data(numbers)) == sizeof(numbers.data()));
}
