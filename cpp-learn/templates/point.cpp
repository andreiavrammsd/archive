#include <array>
#include <cmath>
#include <numeric>
#include <sstream>
#include <cassert>

template<typename T, size_t N>
class Point {
public:
    template<typename... Coordinates>
    explicit Point(Coordinates... c) : coordinates{c...} {
        static_assert(N == sizeof...(c), "");
    }

    const T &operator[](size_t i) const {
        return coordinates.at(i);
    }

    const T *begin() const noexcept {
        return coordinates.data();
    }

    const T *end() const noexcept {
        return coordinates.data() + N;
    }

private:
    std::array<T, N> coordinates;
};

template<typename T, size_t N>
double distance(const Point<T, N> &p1, const Point<T, N> &p2) {
    auto sum = std::accumulate(p1.begin(), p1.end(), T{}, [count = 0, &p2](T a, T b) mutable {
        return a + std::pow(b - p2[count++], 2);
    });

    return std::sqrt(sum);
}

template<typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const Point<T, N> &p) {
    os << '{';
    for (auto c = p.begin(); c != p.end(); ++c) {
        os << *c;

        if (std::next(c) != p.end()) {
            os << ',';
        }
    }
    return os << '}';
}

// float and double comparison
template<typename T>
typename std::conditional<std::is_floating_point<T>::value, bool, void>::type
equals(T a, T b, double epsilon = 0.001) {
    return std::abs(a - b) < epsilon;
}

int main() {
    Point<int, 4> p1{4, 3, 6, -1};
    Point<int, 4> p2{3, -2, 6, -15};

    assert(p1[0] == 4);

    try {
        p1[5];
        assert(false);
    } catch (const std::exception &) {
        static_assert(true, "");
    }

    assert(equals(distance(p1, p2), 14.8997));

    std::stringstream s;
    s << p1;
    assert(s.str() == "{4,3,6,-1}");
}
