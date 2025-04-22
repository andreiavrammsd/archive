/*
 https://www.walletfox.com/course/cheatsheets_cpp.php
 */

#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
#include <set>
#include <cassert>
#include <string>
#include <map>

struct Rect {
    int w;
    int h;
};

//bool operator<(const Rect &r1, const Rect &r2) {
//    return r1.w < r2.w && r1.h < r2.h; // does not satisfy strict weak ordering
//}

bool operator<(const Rect &r1, const Rect &r2) {
    return std::tie(r1.w, r1.h) < std::tie(r2.w, r2.h);
}

int main() {
    /**
     * Populating a vector of pointers
     */
    auto vs = std::vector{
            std::make_shared<int>(3),
    };

    auto vu = std::vector<std::unique_ptr<int>>{};
    vu.push_back(std::make_unique<int>(1));

    /**
     * Initializer list constructor takes priority
     */
    class A {
        int m_x{};
        double m_y{};
    public:
        A(int x, double y) : m_x(x), m_y(y) {}

        explicit A(std::initializer_list<int>) {}

        double get() const {
            return m_x + m_y;
        }
    };
    // A{2, 3.14}; // does not compile
    A(3, 3.14);
    A{3, 314};

    /**
     * Detect undefined behavior
     *
     * Evaluation of constant expressions cannot exhibit undefined behavior.
     */
    std::array<int, 3> arr;
    auto varr = arr[5]; // Compiles - undefined behavior
    std::cout << varr << '\n';

    constexpr std::array<int, 3> carr{};
    // constexpr auto vcarr = carr[5]; // Does not compile
    static_assert(carr.size() == 3);

    /**
     * Inserting user-defined types into std::set
     */
    std::set<Rect> rects = {
            {2, 3},
            {1, 1},
            {1, 4},
            {1, 4},
    };
    assert(rects.size() == 3);

    /**
     * Pre- vs post-increment expressions and value categories
     *
     * ++c is an lvalue
     * c++ is a pure rvalue
     */
    auto c = 0;
    std::cout << &(++c) << '\n'; // compiles
    // std::cout << &(c++) << '\n'; // doesn't compile

    /**
     * How to modify a key
     */
    std::map<int, std::string> m{
            {1, "dog"},
            {2, "cat"},
            {3, "cow"},
    };

    // Before C++17: extra allocation and deallocation
    auto it = m.find(2);
    if (it != m.end()) {
        auto s = it->second;
        m.erase(it);
        m.insert({7, s});
    }
    assert(m[7] == "cat");

    // C++17 introduced extract
    auto nh = m.extract(1);
    if (!nh.empty()) {
        nh.key() = 5;
        m.insert(std::move(nh));
    }
    assert(m[5] == "dog");

    /**
     * Structured binding vs std::tie (C++17)
     */
    struct S {
        S() : a("Hello"), b("world") {}

        std::string a;
        std::string b;

        auto f() {
            return std::tuple{"vim", "ZZZ"};
        }

        void modify1() {
            // auto[a, b] = f(); // shadows original members
        }

        void modify2() {
            // [a, b] = f(); // doesn't compile
        }

        void modify3() {
            std::tie(a, b) = f();
            assert(a == "vim");
            assert(b == "ZZZ");
        }
    };
    S().modify3();
}
