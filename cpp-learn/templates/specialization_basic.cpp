#include <vector>
#include <numeric>
#include <string>
#include <complex>
#include <cassert>

template<typename T>
class Container {
    std::vector<T> v;
public:
    Container(std::initializer_list<T> list) : v{list} {}

    T compute() {
        return std::accumulate(v.begin(), v.end(), T{});
    }
};

template<>
class Container<std::string> { // complete specialization
    std::vector<std::string> v;

    struct decorator {
        std::string separator;

        auto operator()(const std::string &a, const std::string &b) const noexcept {
            return a + b + separator;
        }
    };

public:

    Container(std::initializer_list<std::string> list) : v{list} {}

    std::string compute() {
        return std::accumulate(
                v.begin(),
                v.end(),
                std::string{},
                decorator{","}
        );
    }
};

int main() {
    Container<int> ints{1, 2, 3};
    assert(ints.compute() == 6);

    Container<double> doubles{1, 2, 3};
    assert(doubles.compute() == 6);

    Container<std::string> strings{"a", "b", "c"};
    assert(strings.compute() == "a,b,c,");

    Container<std::complex<double>> complexes{{1, 2},
                                              {2, 3}};
    assert(complexes.compute() == std::complex<double>(3, 5));
}
